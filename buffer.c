#include "buffer.h"

Buffer *new_buffer() {
    Buffer *buf = malloc(sizeof(Buffer));
    buf->name = "Untitled";
    buf->x_view = 0;
    buf->y_view = 0;
    return buf;
}
