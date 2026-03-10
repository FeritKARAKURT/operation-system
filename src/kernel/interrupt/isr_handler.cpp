#include <stdint.h>
#include "../drivers/keyboard.hpp" // Yeni driver'ı ekle
#include "../io.hpp"

extern "C" void isr_handler_c(uint64_t interrupt_num) {
    
    // Klavye Kesmesi
    if (interrupt_num == 33) {
        uint8_t scancode = inb(0x60);
        
        // Mantığı Keyboard Manager'a devret!
        Keyboard::handle_scancode(scancode);

        outb(0x20, 0x20); // EOI
    }
    // Hata Kesmeleri
    else if (interrupt_num < 32) {
        // Hata basma kodları...
        while(1);
    }
    // Diğer EOI işlemleri...
    else if (interrupt_num >= 32) {
        if (interrupt_num >= 40) outb(0xA0, 0x20);
        outb(0x20, 0x20);
    }
}