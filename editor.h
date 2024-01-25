#ifndef EDITOR_H
#define EDITOR_H

#include <ncurses.h>

#include "util.h"
#include "buffer.h"
#include "cursor.h"

typedef struct {
    WINDOW *window;
    Buffer *buffer;
    Cursor *cursor;
    char *mode;
    size_t rows;
    size_t cols;
    size_t margin_l;
    size_t margin_r;
    size_t margin_t;
    size_t margin_b;
    size_t v_scroll_offset;
    size_t h_scroll_offset;
    bool status_line;
    bool line_numbers;
    bool should_exit;
    int last_key;
} Editor;

void editor_init(Editor *editor);                                   // Setup the Editor instance and start ncurses
void editor_deinit(Editor *editor);                                 // Free all allocated memory and close ncurses
Line *editor_get_line(Editor *editor, int row);                     // Get a specific Line in the Buffer
Line *editor_get_cursor_line(Editor *editor);                       // Get the Line at the Cursor's position in the Buffer
Point editor_get_cursor_pos(Editor *editor);                        // Get a Point with the Cursor's row and col
void editor_evaluate_lines(Editor *editor);                         // Recalculate lines from Buffer data
void editor_process_key(Editor *editor, int key);                   // Perform actions based on the pressed key
bool editor_open_file(Editor *editor, char *file_path);             // Load the contents of a file into the Buffer data
void editor_move_cursor(Editor *editor, int x_dir, int y_dir);      // Handle cursor movement within the bounds of the Editor
void editor_scroll_view(Editor *editor);                            // Scroll the Buffer view when necessary
void editor_redraw_screen(Editor *editor);                          // Redraw and refresh the entire screen
void editor_draw_buffer(Editor *editor);                            // Draw all Buffer elements

#endif
