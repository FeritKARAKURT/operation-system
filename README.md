Kernel Project
C++ ve Assembly kullanılarak geliştirilmiş, x86 mimarisini hedefleyen düşük seviyeli bir işletim sistemi çekirdeği.

Proje Hakkında
Bu çalışma, donanım seviyesinde sistem programlama ve çekirdek mimarilerini anlamak amacıyla geliştirilmiştir. Modern görüntüleme teknikleri ve temel donanım sürücülerini içerir.

Özellikler
Özel Bootloader: Sistemi başlatan ve korumalı moda geçişi sağlayan önyükleme mekanizması.

Framebuffer Desteği: VGA metin modu yerine yüksek çözünürlüklü grafik çıktı almayı sağlayan Framebuffer implementasyonu.

GDT & IDT: Global Descriptor Table ve Interrupt Descriptor Table yapılandırması.

Hafıza Yönetimi: Paging (sayfalama) ve kernel-level heap allocator.

Giriş Birimleri: Kesme tabanlı klavye sürücüsü.

Teknik Yığın
Diller: C++, C, x86 Assembly (NASM)

Mimari: x86_64 / x86

Araçlar: GCC Cross-Compiler, Make, Linker scripts

Emülatör: QEMU
