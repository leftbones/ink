#include "keycodes.h"

char* keystr(int keycode) {
    switch (keycode) {
        case CTRL('['): return "Esc";
        case CTRL('a'): return "C-a";
        case CTRL('b'): return "C-b";
        case CTRL('c'): return "C-c";
        case CTRL('x'): return "C-x";
        case KC_H: return "h";
        case KC_J: return "j";
        case KC_K: return "k";
        case KC_L: return "l";
        default: return "NOPE";
    }
}
