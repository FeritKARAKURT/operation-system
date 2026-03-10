#pragma once
#include <stdint.h>
#include <stddef.h>

struct FrameBuffer {
    uint32_t* buffer;  // Renkli piksel dizisi
    size_t width;
    size_t height;
    size_t pitch;      // Byte cinsinden satır uzunluğu
};

// Framebuffer'ı başlatır (bootloader'dan alınan bilgilerle)
void init_framebuffer(FrameBuffer& fb, uint64_t fb_addr, size_t width, size_t height, size_t pitch);

// Tek piksel çizer
void put_pixel(FrameBuffer& fb, int x, int y, uint32_t color);
extern FrameBuffer framebuffer; 