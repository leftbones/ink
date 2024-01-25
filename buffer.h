#ifndef BUFFER_H
#define BUFFER_H

#include <stdlib.h>

typedef struct {
    size_t pos;
    size_t len;
} Line;

typedef struct {
    Line* items;
    size_t count;
    size_t capacity;
} Lines;

typedef struct {
    char* items;
    size_t count;
    size_t capacity;
} Data;

typedef struct {
    char* name;
    Data data;
    Lines lines;
    int offset_x;
    int offset_y;
} Buffer;

Buffer* new_buffer();

#endif
