#ifndef EDITOR_H
#define EDITOR_H

#include <ncurses.h>

#include "buffer.h"
#include "cursor.h"

typedef struct {
    WINDOW* window;
    Buffer* buffer;
    Cursor* cursor;
    char* mode;
    unsigned int rows;
    unsigned int cols;
    bool should_exit;
    int last_key;
} Editor;

void editor_init(Editor* editor);
void editor_deinit(Editor* editor);
Line* editor_get_line(Editor* editor, int row);
Line* editor_get_cursor_line(Editor* editor);
void editor_evaluate_lines(Editor* editor);
void editor_process_key(Editor* editor, int key);
bool editor_open_file(Editor* editor, char* file_path);
void editor_move_cursor(Editor* editor, int x_dir, int y_dir);
void editor_redraw_screen(Editor* editor);
void editor_draw_buffer(Editor* editor);

#endif
