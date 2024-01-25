#include "buffer.h"

Buffer *new_buffer() {
    Buffer *buf = malloc(sizeof(Buffer));
    buf->name = "Untitled";
    buf->scroll_x = 0;
    buf->scroll_y = 0;
    return buf;
}
