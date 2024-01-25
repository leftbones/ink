#ifndef CURSOR_H
#define CURSOR_H

#include <stdlib.h>

typedef struct {
    size_t row;
    size_t col;
    size_t hint;
} Cursor;

Cursor* new_cursor(size_t row, size_t col);

#endif
