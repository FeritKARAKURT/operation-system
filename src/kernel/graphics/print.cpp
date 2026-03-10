#include "print.hpp"
#include "../font/font.hpp"

// renderer.cpp içinde senin fonksiyonun güncellenmiş hali:
void print_str(uint32_t* fb, uint32_t width, uint32_t height, const char* str, int row, int col, uint32_t color) {
    while (*str) {
        if (*str == '\n') {
            row += 16;
            col = 0;
        } else if (*str == ' ') { 
            // BOŞLUK GELİRSE: Orayı doğrudan siyaha boya (Silme garantisi)
            // 8x16'lık alanı temizliyoruz
            for (int y = 0; y < 16; y++) {
                for (int x = 0; x < 8; x++) {
                    // Y koordinatı: row, X koordinatı: col
                    fb[(row + y) * width + (col + x)] = 0x000000;
                }
            }
            col += 8;
        } else {
            put_font_char(fb, width, height, (unsigned char)*str, col, row, color);
            col += 8;
        }
        str++;
    }
}

// Hex fonksiyonunda sorun yok çünkü 0-9 ve A-F karakterleri zaten 127'den küçük.
void print_hex(uint32_t* fb, uint32_t width, uint32_t height, uint64_t value, int row, int col, uint32_t color) {
    const char* hex = "0123456789ABCDEF";
    char buffer[17];
    buffer[16] = '\0';
    for (int i = 15; i >= 0; --i) {
        buffer[i] = hex[value & 0xF];
        value >>= 4;
    }
    // Burada print_str yerine print_font_str kullanmışsın, 
    // eğer print_font_str de aynı unsigned mantığına sahip değilse düzeltmelisin.
    // Ama genelde hex karakterleri (ASCII) sorun çıkarmaz.
    print_font_str(fb, width, height, buffer, col, row, color);
}