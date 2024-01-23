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
    initscr();
    raw();
    noecho();
    nodelay(stdscr, TRUE);
    refresh();

    unsigned int rows, cols;
    getmaxyx(stdscr, rows, cols);
    WINDOW *win = newwin(rows, cols, 0, 0);
    editor->window = win;
    editor->rows = rows;
    editor->cols = cols;

    editor_redraw_screen(editor);
}

void editor_deinit(Editor *editor) {
    Buffer *buf = editor->buffer;
    free(buf->data.items);
    free(buf->lines.items);
    buf->data.items = NULL;
    buf->lines.items = NULL;
    endwin();
}

Line *editor_get_line(Editor *editor, int row) {
    Buffer *buf = editor->buffer;
    Cursor *cur = editor->cursor;
    return &buf->lines.items[row];
}

Line *editor_get_cursor_line(Editor *editor) {
    Buffer *buf = editor->buffer;
    Cursor *cur = editor->cursor;
    return &buf->lines.items[cur->row];
}

void editor_evaluate_lines(Editor *editor) {
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
    dx = clamp(dx, 0, min(cur_line_len, cols - 1));
    dy = clamp(dy, 0, min(buf->lines.count - 1, rows - 2));

    if (x_dir != 0) {
        cur->hint = dx;
    }

    if (y_dir != 0) {
        int d_line_len = max((int)editor_get_line(editor, dy)->len - 1, 0);
        dx = min(d_line_len, cur->hint);
    }

    cur->row = dy;
    cur->col = dx;
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
    for (int i = 1; i < rows - 1; i++) {
        mvwaddch(win, i, 0, '~');
    }

    // Lines
    Lines lines = buf->lines;
    for (int i = 0; i < rows - 1; i++) {
        size_t row = i;
        if (row < lines.count) {
            wmove(win, i, 0);
            wclrtoeol(win);
            Line line = lines.items[row];
            char text[line.len];
            strncpy(text, buf->data.items + line.pos, line.len + 1);
            mvwaddstr(win, i, 0, text);
        }
    }

    // Status
    wmove(win, rows - 1, 0);
    wclrtoeol(win);
    
    char editor_mode[16];
    snprintf(editor_mode, sizeof editor_mode, " %s", editor->mode);
    mvwaddstr(win, rows - 1, 0, editor_mode);

    char cursor_pos[16];
    snprintf(cursor_pos, sizeof cursor_pos, "%lu, %lu ", cur->row, cur->col);
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
