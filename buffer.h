#ifndef BUFFER_H
#define BUFFER_H

#include <stdlib.h>

// Position and length of a substring within Data
typedef struct {
    size_t pos;
    size_t len;
} Line;

// Dynamic array of Lines
typedef struct {
    Line* items;
    size_t count;
    size_t capacity;
} Lines;

// Dynamic array of characters (raw text data of the Buffer contents)
typedef struct {
    char* items;
    size_t count;
    size_t capacity;
} Data;

typedef struct {
    char* name;
    Data data;
    Lines lines;
    int scroll_x;
    int scroll_y;
} Buffer;

Buffer* new_buffer();

#endif
