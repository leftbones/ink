#include <ncurses.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#include "editor.h"
#include "keycodes.h"
#include "defer.h"
#include "darray.h"
#include "util.h"

void editor_init(Editor *editor) {
    // Curses Setup
    initscr();
    raw();
    noecho();
    nodelay(stdscr, TRUE);
    refresh();

    // Config Options
    editor->v_scroll_offset = 5;
    editor->h_scroll_offset = 10;
    editor->status_line = true;
    editor->line_numbers = true;

    // Properties
    unsigned int rows, cols;
    getmaxyx(stdscr, rows, cols);
    WINDOW *win = newwin(rows, cols, 0, 0);
    editor->window = win;
    editor->rows = rows;
    editor->cols = cols;
    editor->margin_l = 4;
    editor->margin_r = 0; // TODO Test that this works
    editor->margin_t = 0;
    editor->margin_b = editor->status_line ? 1 : 0;

    editor->cursor = new_cursor(editor->margin_t, editor->margin_l);

    editor_redraw_screen(editor);
}

// Free up all allocated memory and close ncurses
void editor_deinit(Editor *editor) {
    Buffer *buf = editor->buffer;
    free(buf->data.items);
    free(buf->lines.items);
    buf->data.items = NULL;
    buf->lines.items = NULL;
    endwin();
}

// Get the Line at a specific row in the Buffer
Line *editor_get_line(Editor *editor, int row) {
    Buffer *buf = editor->buffer;
    Cursor *cur = editor->cursor;
    return &buf->lines.items[row + buf->scroll_y];
}

// Get the Line at the Cursor's position
Line *editor_get_cursor_line(Editor *editor) {
    Buffer *buf = editor->buffer;
    Cursor *cur = editor->cursor;
    return &buf->lines.items[cur->row + buf->scroll_y];
}

// Get a Point containing the row and col of the Cursor
Point editor_get_cursor_pos(Editor *editor) {
    Buffer* buf = editor->buffer;
    Cursor* cur = editor->cursor;
    Point pt = point(cur->row + buf->scroll_y - editor->margin_t, cur->col + buf->scroll_x - editor->margin_l);
    return pt;
}

void editor_evaluate_lines(Editor *editor) { // TODO Modify to only evaluate *visible* lines based on the buffer's scroll offsets
    Buffer *buf = editor->buffer;
    buf->lines.count = 0;

    size_t pos = 0;
    for (size_t i = 0; i < buf->data.count; i++) {
        if (buf->data.items[i] == '\n') {
            Line ln = { .pos = pos, .len = i - pos };
            da_append(&buf->lines, ln);
            pos = i + 1;
        }
    }

    if (buf->lines.count == 0) {
        Line ln = { .pos = pos, .len = buf->data.count - pos };
        da_append(&buf->lines, ln);
    }
}

void editor_process_key(Editor *editor, int key) {
    switch (key) {
        case KEY(CTRL('q')): editor->should_exit = true; break;
        case KEY('h'): editor_move_cursor(editor, -1, 0); break;
        case KEY('j'): editor_move_cursor(editor, 0, 1); break;
        case KEY('k'): editor_move_cursor(editor, 0, -1); break;
        case KEY('l'): editor_move_cursor(editor, 1, 0); break;
    }

    editor->last_key = key;
}

bool editor_open_file(Editor *editor, char *file_path) {
    Buffer *buf = editor->buffer;
    
    bool result = true;
    int fd = -1;

    buf->data.count = 0;
    buf->lines.count = 0;

    struct stat statbuf;
    if (stat(file_path, &statbuf) < 0) {
        return_defer(true);
    }

    if ((statbuf.st_mode & S_IFMT) != S_IFREG) {
        fprintf(stderr, "ERROR: %s is not a regular file\n", file_path);
        return_defer(false);
    }

    fd = open(file_path, O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "ERROR: Could not open file %s: %s\n", file_path, strerror(errno));
        return_defer(false);
    }

    size_t file_size = statbuf.st_size;
    da_reserve(&buf->data, file_size);

    ssize_t n = read(fd, buf->data.items, file_size);
    if (n < 0) {
        fprintf(stderr, "ERROR: Failed to read file %s: %s\n", file_path, strerror(errno));
        return_defer(false);
    }

    while ((size_t) n < file_size) {
        ssize_t m = read(fd, buf->data.items + n, file_size - n);
        if (m < 0) {
            fprintf(stderr, "ERROR: Failed to read file %s: %s\n", file_path, strerror(errno));
            return_defer(false);
        }
        n += m;
    }

    buf->data.count = n;
    buf->name = file_path;

    defer:
        if (result) { editor_evaluate_lines(editor); }
        if (fd >= 0) { close(fd); }
        return result;
}

void editor_move_cursor(Editor *editor, int x_dir, int y_dir) {
    Buffer *buf = editor->buffer;
    Cursor *cur = editor->cursor;
    int rows = editor->rows;
    int cols = editor->cols;

    int dx = cur->col + x_dir;
    int dy = cur->row + y_dir;

    int cur_line_len = max((int)editor_get_cursor_line(editor)->len - 1, 0);
    dx = clamp(dx, editor->margin_l, min(cur_line_len + editor->margin_l, cols - editor->margin_r - 1));
    dy = clamp(dy, (int)editor->margin_t, min(buf->lines.count - 1, rows - editor->margin_b - 1));

    if (x_dir != 0) {
        cur->hint = dx;
    }

    if (y_dir != 0) {
        int d_line_len = max((int)editor_get_line(editor, dy)->len - 1, 0) + editor->margin_l;
        dx = min(d_line_len, cur->hint);
    }

    cur->row = dy;
    cur->col = dx;

    editor_scroll_view(editor);
}

void editor_scroll_view(Editor *editor) {
    Buffer *buf = editor->buffer;
    Cursor *cur = editor->cursor;
    size_t v_scroll_offset = editor->v_scroll_offset;
    size_t h_scroll_offset = editor->h_scroll_offset;

    // Vertical
    while (cur->row <= v_scroll_offset - editor->margin_b && buf->scroll_y > 0) {
        buf->scroll_y -= 1;
        cur->row += 1;
    }

    int bottom = editor->rows - editor->margin_b;
    while (cur->row >= bottom - v_scroll_offset && buf->scroll_y + bottom < buf->lines.count) {
        buf->scroll_y += 1;
        cur->row -= 1;
    }

    // Horizontal
    while (cur->col <= h_scroll_offset + editor->margin_l - 1 && buf->scroll_x > 0) { // FIXME Why do we have to subtract 1 from editor->margin_l to get h_scroll_offset to be the same when scrolling left and right?
        buf->scroll_x -= 1;
        cur->col += 1;
    }

    int right = editor->cols - editor->margin_r;
    while (cur->col >= right - h_scroll_offset && buf->scroll_x + right < editor_get_cursor_line(editor)->len + editor->margin_l) {
        buf->scroll_x += 1;
        cur->col -= 1;
    }
}

void editor_redraw_screen(Editor *editor) {
    WINDOW *win = editor->window;
    Buffer *buf = editor->buffer;
    Cursor *cur = editor->cursor;

    editor_draw_buffer(editor);

    wmove(win, cur->row, cur->col);
    wrefresh(win);
}

void editor_draw_buffer(Editor *editor) {
    WINDOW *win = editor->window;
    Buffer *buf = editor->buffer;
    Cursor *cur = editor->cursor;
    int rows = editor->rows;
    int cols = editor->cols;

    // Gutter
    // TODO Maybe implement `gutter_w` so the gutter can respect the left margin and lines can still respect both the margin and the gutter width
    for (int i = editor->margin_t; i < rows - editor->margin_b; i++) {
        mvwaddch(win, i, 0, '~');
    }

    // Lines
    Lines lines = buf->lines;
    Point cursor_pos = editor_get_cursor_pos(editor);
    for (int i = editor->margin_t; i < rows - editor->margin_b; i++) {
        size_t row = (i - editor->margin_t) + buf->scroll_y;
        if (row < lines.count) {
            // Clear line before drawing
            wmove(win, i, 0);
            wclrtoeol(win);

            // Line numbers
            if (editor->line_numbers) { // FIXME This could probably be done much better, maybe move to a dedicated function for use elsewhere
                int digits = numlen(row + 1);
                char padding[(3 - digits) + 1];
                padding[3 - digits] = '\0';
                for (int j = 0; j < 3 - digits; j++) {
                    padding[j] = ' ';
                }
                
                char line_num[4];
                snprintf(line_num, sizeof line_num, "%s%i", padding, (int)(row + 1));

                if (row == cursor_pos.y) {
                    wattrset(win, A_BOLD);
                } else {
                    wattrset(win, A_DIM | A_ITALIC);
                }
                mvwaddstr(win, i, 0, line_num);
                wattrset(win, A_NORMAL);
            }

            // Line text
            Line line = lines.items[row];
            char text[line.len];
            size_t start_col = buf->scroll_x;
            if (start_col < line.len) {
                strncpy(text, buf->data.items + line.pos + start_col, line.len + 1);
                mvwaddstr(win, i, editor->margin_l, text);
            }
        }
    }

    // Status
    if (editor->status_line) {
        wmove(win, rows - 1, 0);
        wclrtoeol(win);
        
        char editor_mode[16];
        wattrset(win, A_BOLD);
        snprintf(editor_mode, sizeof editor_mode, " %s", editor->mode);
        mvwaddstr(win, rows - 1, 0, editor_mode);
        wattrset(win, A_NORMAL);

        char cursor_pos[16];
        Point c_pos = editor_get_cursor_pos(editor);
        snprintf(cursor_pos, sizeof cursor_pos, "%lu, %lu ", c_pos.y, c_pos.x);
        mvwaddstr(win, rows - 1, cols - strlen(cursor_pos) - 8, cursor_pos);

        char last_key[8];
        char *key_str = keystr(editor->last_key);
        if (strsame(key_str, "UNKNOWN")) {
            snprintf(last_key, sizeof last_key, "%c ", (char)editor->last_key);
        } else {
            snprintf(last_key, sizeof last_key, "%s ", key_str);
        }
        mvwaddstr(win, rows - 1, cols - strlen(last_key), last_key);
    }
}
