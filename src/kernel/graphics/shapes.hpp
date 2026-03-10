#pragma once
#include <stdint.h>
#include "framebuffer.hpp"

void draw_pixel(FrameBuffer& fb, int x, int y, uint32_t color);
void fill_rect(FrameBuffer& fb, int x, int y, int width, int height, uint32_t color);
void draw_rect(FrameBuffer& fb, int x, int y, int width, int height, uint32_t color);
void draw_circle(FrameBuffer& fb, int x0, int y0, int radius, uint32_t color);
void fill_circle(FrameBuffer& fb, int x0, int y0, int radius, uint32_t color);
void draw_line(FrameBuffer& fb, int x0, int y0, int x1, int y1, uint32_t color);
void draw_triangle(FrameBuffer& fb, int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
void fill_triangle(FrameBuffer& fb, int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
