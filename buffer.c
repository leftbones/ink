#include "buffer.h"

Buffer *new_buffer() {
    Buffer *buf = malloc(sizeof(Buffer));
    buf->name = "Untitled";
    return buf;
}
