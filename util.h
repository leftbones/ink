#ifndef UTIL_H
#define UTIL_H

#include <string.h>

#define strsame(a, b) (strcmp(a, b) == 0)       // Compare equality of two strings

#define max(a, b) ((a) >= (b) ? (a) : (b))      // Pick the highest of two numbers
#define min(a, b) ((a) <= (b) ? (a) : (b))      // Pick the lowest of two numbers
#define clamp(n, min, max) (n < min ? min : n) > max ? max : (n < min ? min : n)    // Clamp a number between a min and max value

// Represents a row and col position within the Buffer
typedef struct {
    size_t y;
    size_t x;
} Point;

Point point(size_t y, size_t x);    // Creates a new point with the given values

int numlen(int n);                  // Get the number of digits in a number
char *keystr(int keycode);          // Get a nice string for a keycode

#endif
