#include <stdint.h>
#include "../io.hpp"

void pic_remap() {
    // PIC Başlatma (ICW1)
    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    // IDT Offset Ayarları (ICW2)
    outb(0x21, 0x20); // Master PIC -> Interrupt 32'den başlar
    outb(0xA1, 0x28); // Slave PIC  -> Interrupt 40'tan başlar

    // Master/Slave Bağlantısı (ICW3)
    outb(0x21, 0x04);
    outb(0xA1, 0x02);

    // Mod Ayarı (ICW4)
    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    // --- KRİTİK NOKTA: MASKELERİ AÇ ---
    // 0xFD = 1111 1101 (Sadece IRQ 1 - Klavye açık)
    // Eğer hala çalışmazsa burayı 0x00 yaparak her şeyi açmayı dene.
    outb(0x21, 0xFD); 
    outb(0xA1, 0xFF);
}
