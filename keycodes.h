// keycodes.h
#ifndef KCCODES_H
#define KCCODES_H

#define KEY(k) ((int)k)         // Converts a char to a keycode (int)
#define CTRL(c) ((c) & 037)     // Converts a keycode to a ctrl keycode

#define KEY_TAB         9
#define KEY_RETURN      10      // KEY_ENTER is reserved by ncurses
#define KEY_ESCAPE      27
#define KEY_SPACE       32
#define KEY_DELETE      126
#define KEY_BKSP        127     // KEY_BACKSPACE is reserved by ncurses

#endif
