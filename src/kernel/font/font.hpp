#pragma once
#include <stdint.h>

void put_font_char(uint32_t* fb, uint32_t width, uint32_t height, 
                   unsigned char c, int x, int y, uint32_t fg_color);

void print_font_str(uint32_t* fb, uint32_t width, uint32_t height, 
                    const char* str, int x, int y, uint32_t fg_color);