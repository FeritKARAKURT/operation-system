#include "shapes.hpp"
#include "framebuffer.hpp"

// Basit swap fonksiyonu
template<typename T>
inline void swap(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

// Basit abs fonksiyonu
inline int abs(int x) {
    return (x < 0) ? -x : x;
}

// Temel piksel çizimi (put_pixel ile aynı)
void draw_pixel(FrameBuffer& fb, int x, int y, uint32_t color) {
    if (x < 0 || y < 0 || (size_t)x >= fb.width || (size_t)y >= fb.height) return;
    fb.buffer[y * (fb.pitch / 4) + x] = color;
}

// Dolu dikdörtgen
void fill_rect(FrameBuffer& fb, int x, int y, int width, int height, uint32_t color) {
    for (int py = y; py < y + height; ++py) {
        for (int px = x; px < x + width; ++px) {
            draw_pixel(fb, px, py, color);
        }
    }
}

// Dikdörtgen çizimi (sınır çizgisi)
void draw_rect(FrameBuffer& fb, int x, int y, int width, int height, uint32_t color) {
    for (int px = x; px < x + width; ++px) {
        draw_pixel(fb, px, y, color);
        draw_pixel(fb, px, y + height - 1, color);
    }
    for (int py = y; py < y + height; ++py) {
        draw_pixel(fb, x, py, color);
        draw_pixel(fb, x + width - 1, py, color);
    }
}

// Bresenham çizgi algoritması
void draw_line(FrameBuffer& fb, int x0, int y0, int x1, int y1, uint32_t color) {
    int dx = abs(x1 - x0);
    int sx = (x0 < x1) ? 1 : -1;
    int dy = -abs(y1 - y0);
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx + dy;

    while (true) {
        draw_pixel(fb, x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 >= dy) {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx) {
            err += dx;
            y0 += sy;
        }
    }
}

// Çember çizimi (Midpoint Circle)
void draw_circle(FrameBuffer& fb, int x0, int y0, int radius, uint32_t color) {
    int x = radius;
    int y = 0;
    int err = 1 - x;

    while (x >= y) {
        draw_pixel(fb, x0 + x, y0 + y, color);
        draw_pixel(fb, x0 + y, y0 + x, color);
        draw_pixel(fb, x0 - y, y0 + x, color);
        draw_pixel(fb, x0 - x, y0 + y, color);
        draw_pixel(fb, x0 - x, y0 - y, color);
        draw_pixel(fb, x0 - y, y0 - x, color);
        draw_pixel(fb, x0 + y, y0 - x, color);
        draw_pixel(fb, x0 + x, y0 - y, color);
        y++;
        if (err < 0) {
            err += 2 * y + 1;
        } else {
            x--;
            err += 2 * (y - x + 1);
        }
    }
}

// Dolu daire (Midpoint Circle dolgu)
void fill_circle(FrameBuffer& fb, int x0, int y0, int radius, uint32_t color) {
    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
            if (x * x + y * y <= radius * radius) {
                draw_pixel(fb, x0 + x, y0 + y, color);
            }
        }
    }
}

// Üçgen çizimi (sadece kenarları)
void draw_triangle(FrameBuffer& fb, int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
    draw_line(fb, x0, y0, x1, y1, color);
    draw_line(fb, x1, y1, x2, y2, color);
    draw_line(fb, x2, y2, x0, y0, color);
}

// Üçgen doldurma (Scanline algoritması)
void fill_triangle(FrameBuffer& fb, int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
    if (y0 > y1) { swap(y0, y1); swap(x0, x1); }
    if (y1 > y2) { swap(y1, y2); swap(x1, x2); }
    if (y0 > y1) { swap(y0, y1); swap(x0, x1); }

    auto edge_interpolate = [](int y, int y0, int x0, int y1, int x1) -> int {
        if (y1 == y0) return x0;
        return x0 + (x1 - x0) * (y - y0) / (y1 - y0);
    };

    for (int y = y0; y <= y2; ++y) {
        if (y < y1) {
            int xa = edge_interpolate(y, y0, x0, y1, x1);
            int xb = edge_interpolate(y, y0, x0, y2, x2);
            if (xa > xb) swap(xa, xb);
            for (int x = xa; x <= xb; ++x) {
                draw_pixel(fb, x, y, color);
            }
        } else {
            int xa = edge_interpolate(y, y1, x1, y2, x2);
            int xb = edge_interpolate(y, y0, x0, y2, x2);
            if (xa > xb) swap(xa, xb);
            for (int x = xa; x <= xb; ++x) {
                draw_pixel(fb, x, y, color);
            }
        }
    }
}
