#pragma once
#include <stdint.h>

class Color {
private:
    uint32_t value; // 0xAARRGGBB formatında renk

public:
    constexpr Color(uint8_t a, uint8_t r, uint8_t g, uint8_t b)
        : value((a << 24) | (r << 16) | (g << 8) | b) {}

    constexpr Color(uint32_t val = 0xFFFFFFFF) : value(val) {}

    // Renk değerini uint32_t olarak döner
    constexpr uint32_t to_uint32() const { return value; }

    // Renk bileşenleri
    constexpr uint8_t alpha() const { return (value >> 24) & 0xFF; }
    constexpr uint8_t red()   const { return (value >> 16) & 0xFF; }
    constexpr uint8_t green() const { return (value >> 8)  & 0xFF; }
    constexpr uint8_t blue()  const { return value & 0xFF; }

    // Sabit renkler
    static const Color Red;
    static const Color Green;
    static const Color Blue;
    static const Color White;
    static const Color Black;
    static const Color Transparent;
};
