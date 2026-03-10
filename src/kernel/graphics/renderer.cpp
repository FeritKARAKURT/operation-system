#include <stdint.h>
#include "renderer.hpp"
#include "print.hpp"
#include "shapes.hpp"
#include "framebuffer.hpp"
#include <stdarg.h>

#define CURSOR_WIDTH 16
#define CURSOR_HEIGHT 16
#define FONT_WIDTH 8
#define FONT_HEIGHT 16

extern FrameBuffer framebuffer;

namespace renderer {
    static uint32_t cursor_background[CURSOR_HEIGHT][CURSOR_WIDTH];

    void print_str(const char* str, int row, int col, uint32_t color) {
        ::print_str(framebuffer.buffer, framebuffer.width, framebuffer.height, str, row, col, color);
    }

    void print_hex(uint64_t value, int row, int col, uint32_t color) {
        ::print_hex(framebuffer.buffer, framebuffer.width, framebuffer.height, value, row, col, color);
    }

    void reverse_str(char* str, int len) {
        int i = 0, j = len - 1;
        while(i < j) {
            char tmp = str[i];
            str[i] = str[j];
            str[j] = tmp;
            i++; j--;
        }
    }

    int int_to_str(int32_t val, char* buf, int base, bool is_signed) {
        // Basit integer to string dönüşümü
        // base: 10 decimal, 16 hex
        // dönüşen karakter sayısını döner

        int i = 0;
        uint32_t uval;

        if (is_signed && val < 0) {
            uval = (uint32_t)(-val);
        } else {
            uval = (uint32_t)val;
        }

        if (uval == 0) {
            buf[i++] = '0';
            buf[i] = 0;
            return i;
        }

        while (uval != 0) {
            int rem = uval % base;
            buf[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
            uval /= base;
        }

        if (is_signed && val < 0) {
            buf[i++] = '-';
        }

        buf[i] = 0;
        reverse_str(buf, i);

        return i;
    }

    int simple_snprintf(char* buffer, int buffer_size, const char* format, ...) {
        va_list args;
        va_start(args, format);

        int i = 0; // buffer index
        for (const char* p = format; *p && i < buffer_size - 1; p++) {
            if (*p == '%') {
                p++;
                if (*p == 'd') {
                    int val = va_arg(args, int);
                    char numbuf[32];
                    int len = int_to_str(val, numbuf, 10, true);
                    for (int k = 0; k < len && i < buffer_size - 1; k++) {
                        buffer[i++] = numbuf[k];
                    }
                }
                else if (*p == 'u') {
                    unsigned int val = va_arg(args, unsigned int);
                    char numbuf[32];
                    int len = int_to_str(val, numbuf, 10, false);
                    for (int k = 0; k < len && i < buffer_size - 1; k++) {
                        buffer[i++] = numbuf[k];
                    }
                }
                else if (*p == 'x') {
                    unsigned int val = va_arg(args, unsigned int);
                    char numbuf[32];
                    int len = int_to_str(val, numbuf, 16, false);
                    for (int k = 0; k < len && i < buffer_size - 1; k++) {
                        buffer[i++] = numbuf[k];
                    }
                }
                else if (*p == 's') {
                    const char* s = va_arg(args, const char*);
                    while (*s && i < buffer_size - 1) {
                        buffer[i++] = *s++;
                    }
                }
                else {
                    // bilinmeyen format karakteri ise '%' ve karakteri bas
                    buffer[i++] = '%';
                    if (i < buffer_size - 1) {
                        buffer[i++] = *p;
                    }
                }
            } else {
                buffer[i++] = *p;
            }
        }
        buffer[i] = 0;
        va_end(args);
        return i;
    }

    void text_size(const char* str, int& width_out, int& height_out) {
        if (!str) {
            width_out = 0;
            height_out = 0;
            return;
        }

        int len = 0;
        while (str[len] != '\0') len++;

        width_out = len * FONT_WIDTH;
        height_out = FONT_HEIGHT;
    }

    void draw_pixel(int x, int y, uint32_t color) {
        ::draw_pixel(framebuffer, x, y, color);
    }

    void fill_rect(int x, int y, int width, int height, uint32_t color) {
        ::fill_rect(framebuffer, x, y, width, height, color);
    }

    void draw_rect(int x, int y, int width, int height, uint32_t color) {
        ::draw_rect(framebuffer, x, y, width, height, color);
    }

    void draw_line(int x0, int y0, int x1, int y1, uint32_t color) {
        ::draw_line(framebuffer, x0, y0, x1, y1, color);
    }

    void draw_circle(int x0, int y0, int radius, uint32_t color) {
        ::draw_circle(framebuffer, x0, y0, radius, color);
    }

    void fill_circle(int x0, int y0, int radius, uint32_t color) {
        ::fill_circle(framebuffer, x0, y0, radius, color);
    }

    void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
        ::draw_triangle(framebuffer, x0, y0, x1, y1, x2, y2, color);
    }

    void fill_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
        ::fill_triangle(framebuffer, x0, y0, x1, y1, x2, y2, color);
    }

    uint32_t* backbuffer = nullptr;
    
    void backup_cursor_background(int x, int y) {
        for (int dy = 0; dy < CURSOR_HEIGHT; dy++) {
            for (int dx = 0; dx < CURSOR_WIDTH; dx++) {
                int px = x + dx;
                int py = y + dy;
                if (px >= 0 && px < (int)framebuffer.width && py >= 0 && py < (int)framebuffer.height) {
                    cursor_background[dy][dx] = framebuffer.buffer[py * framebuffer.width + px];
                } else {
                    cursor_background[dy][dx] = 0;
                }
            }
        }
    }

    void restore_cursor_background(int x, int y) {
        for (int dy = 0; dy < CURSOR_HEIGHT; dy++) {
            for (int dx = 0; dx < CURSOR_WIDTH; dx++) {
                int px = x + dx;
                int py = y + dy;
                if (px >= 0 && px < (int)framebuffer.width && py >= 0 && py < (int)framebuffer.height) {
                    draw_pixel(px, py, cursor_background[dy][dx]);
                }
            }
        }
    }

    
    void clear_screen(uint32_t color) {
        for (size_t y = 0; y < framebuffer.height; y++) {
            for (size_t x = 0; x < framebuffer.width; x++) {
                framebuffer.buffer[y * (framebuffer.pitch / 4) + x] = color; // Kırmızı
            }
        }    
    }
    
    void clear_line(int row, int length) {
        char spaces[128];
        if (length > 127) length = 127;
        for (int i = 0; i < length; i++) {
            spaces[i] = ' ';
        }
        spaces[length] = 0;
        renderer::print_str(spaces, row, 0, 0x000000); // siyah veya arka plan rengi
    }
    void scroll_up() {
        // Tüm satırları bir karakter boyu (16 px) yukarı taşı
        // Son satırı temizle
        uint32_t* buf = framebuffer.buffer;
        uint32_t width = framebuffer.width;
        uint32_t height = framebuffer.height;

        // 16. satırdan başla ve her şeyi yukarı kopyala
        for (uint32_t y = 16; y < height; y++) {
            for (uint32_t x = 0; x < width; x++) {
                buf[(y - 16) * width + x] = buf[y * width + x];
            }
        }

        // En alttaki 16 satırı siyaha boya (temizle)
        for (uint32_t y = height - 16; y < height; y++) {
            for (uint32_t x = 0; x < width; x++) {
                buf[y * width + x] = 0x000000;
            }
        }
    }
}