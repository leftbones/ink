#include <stdlib.h>
#include <ncurses.h>

#include "main.h"
#include "util.h"
#include "defer.h"
#include "editor.h"

void print_usage() {
    fprintf(stderr, "usage: ink [options] [file_path ..]\n");
    fprintf(stderr, "options:\n");
    fprintf(stderr, "    -h or --help    print this help message\n");
    fprintf(stderr, "    -m              open multiple files, paths separated by spaces\n");
    fprintf(stderr, "    -r              open a file in read-only mode\n");
    fprintf(stderr, "    -s              run in safe mode (config file is ignored)\n");
    fprintf(stderr, "    --version       print the version information\n");
    fprintf(stderr, "    -z              run with zen mode enabled\n");
}

void print_version() {
    fprintf(stderr, "Ink Text Editor %s\n", VERSION);
    fprintf(stderr, "Copyright (c) 2023 Evan J Parker\n");
    fprintf(stderr, "github.com/leftbones/ink\n");
}

int main(int argc, char* argv[]) {
    // Setup
    int result = 0;

    Editor editor = {
        .buffer = new_buffer(),
        .cursor = new_cursor(0, 0),
        .mode = "Normal",
        .last_key = 0
    };

    // Arguments
    char* file_path = NULL;
    if (argc > 1) {
        char* arg = argv[1];

        if (strsame(arg, "-h") || strsame(arg, "--help")) {
            print_usage();
            return_defer(0);
        } else if (strsame(arg, "--version")) {
            print_version();
            return_defer(0);
        } else {
            file_path = argv[1];
        }
    }

    if (!editor_open_file(&editor, file_path)) {
        return_defer(1);
    }

    editor_init(&editor);

    // Input Loop
    int key;
    for (;;) {
        if ((key = wgetch(editor.window)) != ERR) {
            editor_process_key(&editor, key);
        }

        if (editor.should_exit) {
            break;
        } else {
            editor_redraw_screen(&editor);
        }
    }

    getch();

    // Exit
    defer:
        editor_deinit(&editor);
        return EXIT_SUCCESS;
}
