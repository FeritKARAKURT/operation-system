#pragma once
#include <stdint.h>

// Porttan Veri Yaz (Output Byte)
static inline void outb(uint16_t port, uint8_t val) {
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

// Porttan Veri Oku (Input Byte)
static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

// Gerekirse bekleme yapmak için (Eski donanımlar için)
static inline void io_wait() {
    outb(0x80, 0);
}