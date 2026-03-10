#pragma once

#include <stdint.h>

void print_str(uint32_t* fb, uint32_t width, uint32_t height, const char* str, int col, int row, uint32_t rowor);
void print_hex(uint32_t* fb, uint32_t width, uint32_t height, uint64_t value, int col, int row, uint32_t color);
