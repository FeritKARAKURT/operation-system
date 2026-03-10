#pragma once
#include <stdint.h>

namespace renderer {
    // Print
    void print_str(const char* str, int row, int col, uint32_t color);
    void print_hex(uint64_t value, int row, int col, uint32_t color);
    void reverse_str(char* str, int len);
    int int_to_str(int32_t val, char* buf, int base, bool is_signed);
    int simple_snprintf(char* buffer, int buffer_size, const char* format, ...);
    void text_size(const char* str, int& width_out, int& height_out);

    // Simle Shapes
    void draw_pixel(int x, int y, uint32_t color);
    void fill_rect(int x, int y, int width, int height, uint32_t color);
    void draw_rect(int x, int y, int width, int height, uint32_t color);
    void draw_line(int x0, int y0, int x1, int y1, uint32_t color);
    void draw_circle(int x0, int y0, int radius, uint32_t color);
    void fill_circle(int x0, int y0, int radius, uint32_t color);
    void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
    void fill_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);

    // Cursor
    void draw_cursor(int x, int y, uint32_t color);
    void erase_cursor(int x, int y);
    void backup_cursor_background(int x, int y);
    void restore_cursor_background(int x, int y);

    // Clear Screen
    void clear_screen(uint32_t color);
    void clear_line(int row, int length);

    // Kaydırma
    void scroll_up();
}