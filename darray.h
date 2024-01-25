// Dynamic Array
// Borrowed (aka shamelessly stolen) from https://github.com/tsoding/noed

#ifndef DARRAY_H
#define DARRAY_H

#define ITEMS_INIT_CAPACITY

#define da_append(da, item) do {                                                            \
    if ((da)->count >= (da)->capacity) {                                                    \
        (da)->capacity = (da)->capacity == 0 ? ITEMS_INIT_CAPACITY : (da)->capacity * 2;    \
        (da)->items = realloc((da)->items, (da)->capacity * sizeof(*(da)->items));          \
    }                                                                                       \
    (da)->items[(da)->count++] = (item);                                                    \
} while (0)

#define da_reserve(da, desired_capacity) do {                                               \
    if ((da)->capacity < desired_capacity) {                                                \
        (da)->capacity = desired_capacity;                                                  \
        (da)->items = realloc((da)->items, (da)->capacity * sizeof(*(da)->items));          \
    }                                                                                       \
} while (0)

#endif
