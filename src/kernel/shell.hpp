#pragma once
#include <stdint.h>

class Shell {
public:
    void init();
    void on_key_press(char c);

private:
    void execute_command();
    void new_line(); // Satır başı ve scrolling kontrolü

    char buffer[256];
    int index;
    int cursor_x;
    int cursor_y;
};