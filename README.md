C++ ve Assembly ile geliştirilmiş, x86 mimarisini hedefleyen, minimal ve hobi amaçlı bir işletim sistemi çekirdeği.

📌 Proje Hakkında
Bu proje, bir bilgisayarın en temel seviyede nasıl çalıştığını anlamak ve donanım ile doğrudan iletişim kuran bir sistem inşa etmek amacıyla geliştirilmiştir. Kendi bootloader'ından bellek yönetimine kadar birçok bileşeni sıfırdan içermektedir.

✨ Öne Çıkan Özellikler
Özel Bootloader: Grub veya benzeri bir bootloader yerine özelleştirilmiş önyükleme mekanizması.

GDT & IDT: Global Descriptor Table ve Interrupt Descriptor Table yapılandırması.

Bellek Yönetimi: Paging ve basit bir heap allocator.

VGA Sürücüsü: Ekranda metin yazdırma ve renk desteği.

Klavye Sürücüsü: Kesme (interrupt) tabanlı temel giriş desteği.

🛠️ Teknik Detaylar
Proje temel olarak aşağıdaki teknolojileri kullanmaktadır:

Diller: C++, C, x86 Assembly (NASM)

Mimari: x86 (32-bit/64-bit)

Derleyici: gcc-cross-compiler, nasm

Emülatör: QEMU veya Bochs

🚀 Başlangıç
Projeyi kendi yerel makinenizde çalıştırmak için aşağıdaki adımları izleyebilirsiniz.

Gereksinimler
Sisteminizde aşağıdaki araçların kurulu olduğundan emin olun:

Bash
sudo apt install nasm qemu-system-x86 build-essential
Derleme ve Çalıştırma
Depoyu klonlayın:

Bash
git clone https://github.com/kullanici-adin/isletim-sistemi-adi.git
cd isletim-sistemi-adi
Projeyi derleyin (Makefile varsa):

Bash
make
QEMU üzerinde test edin:

Bash
make run
📂 Dosya Yapısı
/src: Tüm kaynak kodlar (C++ ve ASM).

/boot: Bootloader dosyaları.

/include: Header (.h) dosyaları.

/scripts: Derleme ve ISO oluşturma betikleri.

🛠️ Gelecek Planları (Roadmap)
[ ] Dosya sistemi desteği (FAT32/ext2).

[ ] Multitasking (Çoklu görev) desteği.

[ ] Kullanıcı modu (User mode) ve syscalls.

[ ] Grafik arayüzü (GUI) başlangıcı.

🤝 Katkıda Bulunma
Katkı sağlamak isterseniz lütfen bir "Issue" açın veya doğrudan bir "Pull Request" gönderin. Her türlü fikir ve geliştirmeye açığım!

📄 Lisans
Bu proje MIT Lisansı altında lisanslanmıştır. Daha fazla bilgi için LICENSE dosyasına göz atabilirsiniz.
