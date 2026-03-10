#include "shell.hpp"
#include "graphics/renderer.hpp"
#include "utils.hpp"
#include "io.hpp"

// Global nesne (Kernel ve Bridge tarafından erişilir)
extern Shell main_shell;

// Keyboard sürücüsünün çağıracağı köprü fonksiyonu
void shell_input_bridge(char c) {
    main_shell.on_key_press(c);
}

void Shell::init() {
    memset(buffer, 0, 256);
    index = 0;
    cursor_x = 10;
    cursor_y = 10;
    
    renderer::clear_screen(0x000000);
    renderer::print_str("TURK-OS SHELL v1.2", cursor_y, cursor_x, 0x00FFFF);
    new_line();
}

void Shell::new_line() {
    cursor_x = 10;
    cursor_y += 16;

    // SCROLLING KONTROLÜ: Ekranın altına ulaşıldıysa kaydır
    // (Genelde 600px yükseklik için 580 sınırı iyidir)
    if (cursor_y >= 580) {
        renderer::scroll_up();
        cursor_y -= 16; // Son satırda kal
    }
    
    renderer::print_str("OS> ", cursor_y, cursor_x, 0x00FF00);
    cursor_x += 32;
}

void Shell::on_key_press(char c) {
    // 1. BACKSPACE
    if (c == '\b') {
        if (index > 0) {
            // 1. Mantıksal indeksi bir geri al
            index--;
            buffer[index] = 0;

            // 2. İmleci (X koordinatını) bir karakter geri çek
            // Senin kodunda 'col' değerine karşılık gelen değişkeni 8 azaltıyoruz
            cursor_x -= 8;

            // 3. Silme işlemini yap
            // Senin print_str fonksiyonun: (fb, w, h, str, row, col, color)
            // DİKKAT: row (Y), col (X) sırasıyla gönderiyoruz
            renderer::print_str(" ", cursor_y, cursor_x, 0x000000); 
        }
        return;
    }
    // 2. ENTER
    if (c == '\n') {
        execute_command();
        return;
    }

    // 3. KARAKTER EKLEME
    if (index < 255) {
        buffer[index++] = c;
        buffer[index] = 0;

        char s[2] = {c, '\0'};
        renderer::print_str(s, cursor_y, cursor_x, 0xFFFFFF);
        cursor_x += 8;

        // Satır sonu kontrolü
        if (cursor_x >= 780) {
            new_line();
        }
    }
}

void Shell::execute_command() {
    // Komutu işlemeden önce promptun hemen altına inme
    // (Bazı komutlar çıktı üretmeyebilir, o yüzden dikkatli yönetiyoruz)
    
    if (strcmp(buffer, "temizle") == 0) {
        renderer::clear_screen(0x000000);
        cursor_y = -6; // new_line içinde 10'a çekilecek
    }
    else if (strcmp(buffer, "reboot") == 0) {
        renderer::print_str("Yeniden baslatiliyor...", cursor_y + 16, 10, 0xFF0000);
        outb(0x64, 0xFE);
    }
    else if (strcmp(buffer, "yardim") == 0) {
        cursor_y += 16;
        renderer::print_str("Komutlar: temizle, reboot, kimsin, yardim", cursor_y, 10, 0xFFFF00);
    }
    else if (strcmp(buffer, "kimsin") == 0) {
        cursor_y += 16;
        renderer::print_str("Ben bare-metal C++ ile yazilmis bir Kernel'im.", cursor_y, 10, 0x00FFFF);
    }
    else if (strlen(buffer) > 0) {
        cursor_y += 16;
        renderer::print_str("Hata: ", cursor_y, 10, 0xFF0000);
        renderer::print_str(buffer, cursor_y, 10 + (6 * 8), 0xFFFFFF);
        renderer::print_str(" bulunamadi.", cursor_y, 10 + (6 * 8) + (strlen(buffer) * 8), 0xFF0000);
    }

    // Sıfırla ve yeni satıra geç
    memset(buffer, 0, 256);
    index = 0;
    new_line();
}