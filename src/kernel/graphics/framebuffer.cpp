#include "framebuffer.hpp"
FrameBuffer framebuffer;
void init_framebuffer(FrameBuffer& fb, uint64_t fb_addr, size_t width, size_t height, size_t pitch) {
    fb.buffer = (uint32_t*)fb_addr;
    fb.width = width;
    fb.height = height;
    fb.pitch = pitch;
}

void put_pixel(FrameBuffer& fb, int x, int y, uint32_t color) {
    if (x < 0 || y < 0 || (size_t)x >= fb.width || (size_t)y >= fb.height) return;
    // pitch byte cinsinden, 4 byte = 1 uint32_t
    fb.buffer[y * (fb.pitch / 4) + x] = color;
}
