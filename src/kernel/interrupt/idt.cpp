#include "idt.hpp"

namespace interrupt {
    IDTEntry idt_table[256];
    IDTPointer idt_ptr;
    extern "C" void* isr_stub_table[];

    void set_gate(int num, uint64_t handler, uint8_t type) {
        idt_table[num].offset_low  = (uint16_t)handler;
        idt_table[num].offset_mid  = (uint16_t)(handler >> 16);
        idt_table[num].offset_high = (uint32_t)(handler >> 32);
        idt_table[num].selector    = 0x08;
        idt_table[num].ist         = 0;
        idt_table[num].type_attr   = type;
        idt_table[num].reserved    = 0;
    }

    void init() {
        for (int i = 0; i < 256; i++) {
            set_gate(i, (uint64_t)isr_stub_table[i], 0x8E);
        }
        idt_ptr.limit = (sizeof(IDTEntry) * 256) - 1;
        idt_ptr.base  = (uint64_t)&idt_table;
        load_idt(&idt_ptr);
    }
}