#include "keyboard.hpp"

namespace Keyboard {
    State kbd_state = {false, false, false, false};
    
    // Şu anki aktif fonksiyon adresi (Başlangıçta boş)
    static KeyHandlerFunc current_handler = 0;

    void set_active_handler(KeyHandlerFunc handler) {
        current_handler = handler;
    }

    const unsigned char keymap_tr_lower[] = {
        0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '*', '-', '\b', 
        '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 0xFD, 'o', 'p', 0xF0, 0xFC, '\n',
        0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 0xFE, 'i', ',', 0, '\\',
        'z', 'x', 'c', 'v', 'b', 'n', 'm', 0xF6, 0xE7, '.', ',', 0, '*', 0, ' '
    };

    const unsigned char keymap_tr_upper[] = {
        0, 27, '!', '\'', '^', '+', '%', '&', '/', '(', ')', '=', '?', '_', '\b',
        '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', 0xD0, 0xDC, '\n',
        0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 0xDE, 0xDD, ';', 0, '\\',
        'Z', 'X', 'C', 'V', 'B', 'N', 'M', 0xD6, 0xC7, ':', ';', 0, '*', 0, ' '
    };

    void init() { current_handler = 0; }

    void handle_scancode(uint8_t scancode) {
        if (scancode & 0x80) {
            uint8_t released = scancode & 0x7F; 
            if (released == 0x2A || released == 0x36) kbd_state.shift = false;
            return; 
        }
        if (scancode == 0x2A || scancode == 0x36) { kbd_state.shift = true; return; }
        if (scancode == 0x3A) { kbd_state.caps_lock = !kbd_state.caps_lock; return; }

        unsigned char ascii = 0;
        if (scancode < sizeof(keymap_tr_lower)) {
            bool uppercase = kbd_state.shift ^ kbd_state.caps_lock;
            ascii = uppercase ? keymap_tr_upper[scancode] : keymap_tr_lower[scancode];
        }

        // Eğer bir handler atanmışsa veriyi oraya fırlat
        if (ascii != 0 && current_handler != 0) {
            current_handler((char)ascii);
        }
    }
}