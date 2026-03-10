#pragma once
#include <stdint.h>

namespace gdt {
    // 8 Byte'lık GDT Girişi
    struct [[gnu::packed]] GDTEntry {
        uint16_t limit_low;
        uint16_t base_low;
        uint8_t base_middle;
        uint8_t access;      // Burası sihirli hex kodunun (0x9A, 0x92) geleceği yer
        uint8_t granularity;
        uint8_t base_high;
    };

    // CPU'ya tablonun yerini söyleyen yapı
    struct [[gnu::packed]] GDTPointer {
        uint16_t limit;
        uint64_t base;
    };

    void init();
}

extern "C" void load_gdt(gdt::GDTPointer* gdt_ptr);