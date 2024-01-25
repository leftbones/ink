#include "buffer.h"

Buffer *new_buffer() {
    Buffer *buf = malloc(sizeof(Buffer));
    buf->name = "Untitled";
    buf->offset_x = 0;
    buf->offset_y = 0;
    return buf;
}
