#include <stdlib.h>
#include "cursor.h"

Cursor *new_cursor(size_t row, size_t col) {
    Cursor *cursor = malloc(sizeof(Cursor));
    cursor->row = row;
    cursor->col = col;
    cursor->hint = col;
    return cursor;
}
