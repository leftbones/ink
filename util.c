#include <limits.h>
#include "keycodes.h"
#include "util.h"

Point point(size_t y, size_t x) {
    Point pt = {
        .y = y,
        .x = x,
    };
    return pt;
}

int numlen(int n) {
    if (n < 0) return numlen((n == INT_MIN) ? INT_MAX: -n);
    if (n < 10) return 1;
    return 1 + numlen(n / 10);
}

char *keystr(int keycode) {
    switch (keycode) {
        // Special Keys
        case KEY_TAB:       return "Tab";
        case KEY_RETURN:    return "Return";
        case KEY_ESCAPE:    return "Esc";
        case KEY_SPACE:     return "Space";
        case KEY_DELETE:    return "Delete";
        case KEY_BKSP:      return "Bksp";

        // Ctrl + Character
        case CTRL('a'):     return "C-a";
        case CTRL('b'):     return "C-b";
        case CTRL('c'):     return "C-c";
        case CTRL('d'):     return "C-d";
        case CTRL('e'):     return "C-e";
        case CTRL('f'):     return "C-f";
        case CTRL('g'):     return "C-g";
        case CTRL('h'):     return "C-h";
        // case CTRL('i'):     return "C-i";    Same as 'Tab'
        // case CTRL('j'):     return "C-j";    Same as 'Enter'
        case CTRL('k'):     return "C-k";
        case CTRL('l'):     return "C-l";
        // case CTRL('m'):     return "C-m";    Same as 'Enter'
        case CTRL('n'):     return "C-n";
        case CTRL('o'):     return "C-o";
        case CTRL('p'):     return "C-p";
        case CTRL('q'):     return "C-q";
        case CTRL('r'):     return "C-r";
        case CTRL('s'):     return "C-s";
        case CTRL('t'):     return "C-t";
        case CTRL('u'):     return "C-u";
        case CTRL('v'):     return "C-v";
        case CTRL('w'):     return "C-w";
        case CTRL('x'):     return "C-x";
        case CTRL('y'):     return "C-y";
        case CTRL('z'):     return "C-z";

        // Lowercase Letters
        case KEY('a'):      return "a";
        case KEY('b'):      return "b";
        case KEY('c'):      return "c";
        case KEY('d'):      return "d";
        case KEY('e'):      return "e";
        case KEY('f'):      return "f";
        case KEY('g'):      return "g";
        case KEY('h'):      return "h";
        case KEY('i'):      return "i";
        case KEY('j'):      return "j";
        case KEY('k'):      return "k";
        case KEY('l'):      return "l";
        case KEY('m'):      return "m";
        case KEY('n'):      return "n";
        case KEY('o'):      return "o";
        case KEY('p'):      return "p";
        case KEY('q'):      return "q";
        case KEY('r'):      return "r";
        case KEY('s'):      return "s";
        case KEY('t'):      return "t";
        case KEY('u'):      return "u";
        case KEY('v'):      return "v";
        case KEY('w'):      return "w";
        case KEY('x'):      return "x";
        case KEY('y'):      return "y";
        case KEY('z'):      return "z";

        // Uppercase Letters
        case KEY('A'):      return "A";
        case KEY('B'):      return "B";
        case KEY('C'):      return "C";
        case KEY('D'):      return "D";
        case KEY('E'):      return "E";
        case KEY('F'):      return "F";
        case KEY('G'):      return "G";
        case KEY('H'):      return "H";
        case KEY('I'):      return "I";
        case KEY('J'):      return "J";
        case KEY('K'):      return "K";
        case KEY('L'):      return "L";
        case KEY('M'):      return "M";
        case KEY('N'):      return "N";
        case KEY('O'):      return "O";
        case KEY('P'):      return "P";
        case KEY('Q'):      return "Q";
        case KEY('R'):      return "R";
        case KEY('S'):      return "S";
        case KEY('T'):      return "T";
        case KEY('U'):      return "U";
        case KEY('V'):      return "V";
        case KEY('W'):      return "W";
        case KEY('X'):      return "X";
        case KEY('Y'):      return "Y";
        case KEY('Z'):      return "Z";

        // Numbers
        case KEY('1'):      return "1";
        case KEY('2'):      return "2";
        case KEY('3'):      return "3";
        case KEY('4'):      return "4";
        case KEY('5'):      return "5";
        case KEY('6'):      return "6";
        case KEY('7'):      return "7";
        case KEY('8'):      return "8";
        case KEY('9'):      return "9";
        case KEY('0'):      return "0";

        // Symbols
        case KEY('!'):      return "!";
        case KEY('@'):      return "@";
        case KEY('#'):      return "#";
        case KEY('$'):      return "$";
        case KEY('%'):      return "%";
        case KEY('^'):      return "^";
        case KEY('&'):      return "&";
        case KEY('*'):      return "*";
        case KEY('('):      return "(";
        case KEY(')'):      return ")";
        case KEY('-'):      return "-";
        case KEY('_'):      return "_";
        case KEY('='):      return "=";
        case KEY('+'):      return "+";
        case KEY('`'):      return "`";
        // case KEY('~'):      return "~";   Same as 'Delete'
        case KEY('['):      return "[";
        case KEY(']'):      return "]";
        case KEY('{'):      return "{";
        case KEY('}'):      return "}";
        case KEY('\\'):      return "\\";
        case KEY('|'):      return "|";
        case KEY(';'):      return ";";
        case KEY(':'):      return ":";
        case KEY('\''):      return "'";
        case KEY('"'):      return "\"";
        case KEY(','):      return ",";
        case KEY('<'):      return "<";
        case KEY('.'):      return ".";
        case KEY('>'):      return ">";
        case KEY('/'):      return "/";
        case KEY('?'):      return "?";

        default:            return "UNKNOWN";   // Unrecognized keycode, should never happen, this is just here to make the compiler happy
    }
}
