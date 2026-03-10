#include "gdt.hpp"
#include "../graphics/renderer.hpp"

namespace gdt {
    // 3 girişimiz olacak: NULL, Code, Data
    GDTEntry gdt_table[3];
    GDTPointer gdt_ptr;

    void set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
        gdt_table[num].base_low    = (base & 0xFFFF);
        gdt_table[num].base_middle = (base >> 16) & 0xFF;
        gdt_table[num].base_high   = (base >> 24) & 0xFF;

        gdt_table[num].limit_low   = (limit & 0xFFFF);
        gdt_table[num].granularity = (limit >> 16) & 0x0F;

        gdt_table[num].granularity |= gran & 0xF0;
        gdt_table[num].access      = access;
    }

    void init() {
        // 1. Giriş: NULL (Zorunlu olarak boş)
        set_gate(0, 0, 0, 0, 0);

        // 2. Giriş: Kernel Code (0x9A access, 0xA0 granularity)
        set_gate(1, 0, 0xFFFFF, 0x9A, 0xA0);

        // 3. Giriş: Kernel Data (0x92 access, 0xA0 granularity)
        set_gate(2, 0, 0xFFFFF, 0x92, 0xA0);

        // CPU'ya gidecek tabelayı hazırla
        gdt_ptr.limit = (sizeof(GDTEntry) * 3) - 1;
        gdt_ptr.base  = (uint64_t)&gdt_table;
        // Assembly fonksiyonunu çağır (bir sonraki adımda yazacağız)
        load_gdt(&gdt_ptr);
    }
}