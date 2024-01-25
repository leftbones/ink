# Notes

## Good Ideas

* Configuration through some kind of `.inkrc` file (located at `~/.config/ink/`), preferably without using an external config library
* Plugin API for users to write and "install" their own custom plugins or share plugins with others
* Graphical renderer frontend similar to Emacs that could allow for more extensive customization, custom fonts, text sizes, images, etc
* Prompt can be displayed as a "Command Palette" (like VSCode) or "Prompt Line" (like Vim)
* Prompt displays possible commands as you type like Helix or (Doom) Emacs does extremely useful

## Stupid Ideas

* Plugin API written in C
    - Plugins could be structured like "~/.config/ink/plugins/$MyPlugin"
    - Source and header files inside a plugin folder would be compiled at runtime
    - Code files could be hashed and compared at runtime to avoid redundant compilation, only re-compiling when the hash changes
* GitHub Issues accessible from inside the editor so if someone wants to help out they can easily be taken to an area of the source code marked with a "FIXME" or something
* Maybe a config similar to Vim's where each line is called as an editor command at runtime with the a function like `set(property, value)`
    - `set` command examples:
        - `set(status_line, false)` - disables the status line
        - `set(line_numbers, "relative")` - sets line numbers to relative
        - `set(v_scroll_offset, 10)` - modifies the default vertical scroll offset
