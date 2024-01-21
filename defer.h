#ifndef DEFER_H
#define DEFER_H

#define return_defer(value) do { result = (value); goto defer; } while(0)

#endif
