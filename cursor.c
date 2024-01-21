#include <stdlib.h>
#include "cursor.h"

Cursor* new_cursor(unsigned int row, unsigned int col) {
    Cursor* cursor = malloc(sizeof(Cursor));
    cursor->row = row;
    cursor->col = col;
    cursor->hint = col;
    return cursor;
}
