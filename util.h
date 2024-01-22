#ifndef UTIL_H
#define UTIL_H

#include <string.h>
#define strsame(a, b) (strcmp(a, b) == 0)

#define max(a, b) ((a) >= (b) ? (a) : (b))
#define min(a, b) ((a) <= (b) ? (a) : (b))
#define clamp(n, min, max) (n < min ? min : n) > max ? max : (n < min ? min : n)

char *keystr(int keycode);

#endif
