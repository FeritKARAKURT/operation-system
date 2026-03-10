#include <stdint.h>

extern "C" unsigned char _binary_font_psf_start[];

#define PSF1_MAGIC0 0x36
#define PSF1_MAGIC1 0x04

struct PSF1_Header {
    uint8_t magic[2];
    uint8_t mode;
    uint8_t charsize;
};

static PSF1_Header* get_font_header() {
    return (PSF1_Header*)_binary_font_psf_start;
}

// DÜZELTME 1: Parametreyi 'unsigned char' yaptık.
// Artık 254 (-2) olarak değil, gerçekten 254 olarak işlem görecek.
static uint8_t* get_glyph(unsigned char c, PSF1_Header* header) {
    return (uint8_t*)_binary_font_psf_start + sizeof(PSF1_Header) + (c * header->charsize);
}

// DÜZELTME 2: Burada da 'unsigned char' yaptık.
void put_font_char(uint32_t* fb, uint32_t width, uint32_t height,
                   unsigned char c, int x, int y, uint32_t fg_color) {
    
    PSF1_Header* header = get_font_header();
    if (header->magic[0] != PSF1_MAGIC0 || header->magic[1] != PSF1_MAGIC1)
        return;

    // get_glyph artık doğru adresi döndürecek
    uint8_t* glyph = get_glyph(c, header);

    for (uint8_t dy = 0; dy < header->charsize; dy++) {
        uint8_t line = glyph[dy];
        for (uint8_t dx = 0; dx < 8; dx++) {
            if ((line >> (7 - dx)) & 1) {
                int px = x + dx;
                int py = y + dy;
                if ((uint32_t)px < width && (uint32_t)py < height) {
                    fb[py * width + px] = fg_color;
                }
            }
        }
    }
}

// String basma fonksiyonu
void print_font_str(uint32_t* fb, uint32_t width, uint32_t height,
                    const char* str, int x, int y, uint32_t fg_color) {
    
    PSF1_Header* header = get_font_header();
    if (header->magic[0] != PSF1_MAGIC0 || header->magic[1] != PSF1_MAGIC1)
        return;

    int cursor_x = x;
    int cursor_y = y;
    int char_height = header->charsize;
    int char_width = 8;

    while (*str) {
        if (*str == '\n') {
            cursor_y += char_height;
            cursor_x = x;
        } else {
            // DÜZELTME 3: *str'yi fonksiyona yollarken (unsigned char) olarak zorluyoruz.
            // Bu "Casting" işlemi en kritik noktadır.
            put_font_char(fb, width, height, (unsigned char)(*str), cursor_x, cursor_y, fg_color);
            cursor_x += char_width;
        }
        str++;
    }
}