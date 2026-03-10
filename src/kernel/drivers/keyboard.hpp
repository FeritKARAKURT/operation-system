#pragma once
#include <stdint.h>
#include "input_type.hpp"

namespace Keyboard {
    struct State {
        bool shift, ctrl, alt, caps_lock;
    };

    void init();
    void handle_scancode(uint8_t scancode);
    
    // Odağı değiştiren ana fonksiyon
    void set_active_handler(KeyHandlerFunc handler);

    bool is_shift_pressed();
}