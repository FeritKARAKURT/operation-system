#include <stdint.h>
#include <stddef.h> // size_t tanımı buradadır

// Eğer <stddef.h> hata verirse bu satırı açıp üsttekini silebilirsin:
// typedef unsigned long long size_t;

// İki kelime aynı mı diye bakar (0 dönerse aynıdır)
int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

// Kelime uzunluğunu bulur (Shell için gerekli)
int strlen(const char* str) {
    int len = 0;
    while (str[len])
        len++;
    return len;
}

// Hafızayı temizler (Buffer'ı sıfırlamak için)
void memset(void* ptr, int value, size_t num) {
    unsigned char* p = (unsigned char*)ptr;
    while (num--) {
        *p++ = (unsigned char)value;
    }
}