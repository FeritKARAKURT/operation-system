# --- Araçlar ve Yollar ---
CC           := gcc
LD           := ld
OBJCOPY      := objcopy
KCC          := x86_64-elf-g++
EFIINC       := /usr/include/efi
EFILIB       := /usr/lib

# --- Derleme Bayrakları ---
CFLAGS_BOOT  := -I$(EFIINC) -I$(EFIINC)/x86_64 -O2 -fpic -fshort-wchar -ffreestanding
LDFLAGS_BOOT := -nostdlib -znocombreloc -T /usr/lib/elf_x86_64_efi.lds -shared -Bsymbolic -L$(EFILIB)
LIBS_BOOT    := -lefi -lgnuefi

KCFLAGS      := -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -fno-stack-protector -nostdlib
KLDFLAGS     := -T linker.ld -nostdlib -static

# --- Dizinler ---
SRC_DIR      := src
BOOT_DIR     := $(SRC_DIR)/boot
KERNEL_DIR   := $(SRC_DIR)/kernel
BUILD_DIR    := build
BUILD_ASM_DIR := $(BUILD_DIR)/asm

# --- Font Binary ---
FONT_BIN     := font.psf
FONT_OBJ     := $(BUILD_DIR)/font_psf.o

# --- Bootloader ---
BOOT_SRCS    := $(BOOT_DIR)/main.c
BOOT_OBJS    := $(BUILD_DIR)/main.o
BOOT_SO      := $(BUILD_DIR)/main.so
EFI_APP      := $(BUILD_DIR)/main.efi

# --- Kernel Kaynakları Otomatik Toplama ---
KERNEL_CPPS  := $(shell find $(KERNEL_DIR) -name "*.cpp")
KERNEL_ASMS  := $(shell find $(KERNEL_DIR) -name "*.asm")
KERNEL_CPP_OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(KERNEL_CPPS))
KERNEL_ASM_OBJS := $(patsubst $(SRC_DIR)/%.asm, $(BUILD_ASM_DIR)/%.o, $(KERNEL_ASMS))
KERNEL_OBJS  := $(KERNEL_CPP_OBJS) $(KERNEL_ASM_OBJS)

KERNEL_APP   := $(BUILD_DIR)/kernel.elf

# --- Hedefler ---
.PHONY: all clean buildimg runimg

all: $(BUILD_DIR) $(EFI_APP) $(FONT_OBJ) $(KERNEL_APP) buildimg

# --- Bootloader Derleme ---
$(BOOT_OBJS): $(BOOT_SRCS) | $(BUILD_DIR)
	$(CC) $(CFLAGS_BOOT) -c $< -o $@

$(BOOT_SO): $(BOOT_OBJS)
	$(LD) $(LDFLAGS_BOOT) /usr/lib/crt0-efi-x86_64.o $< -o $@ $(LIBS_BOOT)

$(EFI_APP): $(BOOT_SO)
	$(OBJCOPY) -j .text -j .sdata -j .data -j .dynamic -j .dynsym -j .rela -j .reloc --target=efi-app-x86_64 $< $@

# --- Font binary objesi oluştur ---
$(FONT_OBJ): $(FONT_BIN) | $(BUILD_DIR)
	$(OBJCOPY) -I binary -O elf64-x86-64 -B i386:x86-64 $< $@

# --- C++ Dosyaları Derleme ---
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(KCC) $(KCFLAGS) -c $< -o $@

# --- ASM Dosyaları Derleme ---
$(BUILD_ASM_DIR)/%.o: $(SRC_DIR)/%.asm
	@mkdir -p $(dir $@)
	nasm -f elf64 $< -o $@

# --- Kernel Uygulamasını Bağla ---
$(KERNEL_APP): $(KERNEL_OBJS) $(FONT_OBJ)
	$(KCC) $(KLDFLAGS) -o $@ $^

# --- Build klasörü oluşturma ---
$(BUILD_DIR):
	mkdir -p $@

# --- Disk İmajı ---
buildimg: $(EFI_APP) $(KERNEL_APP)
	dd if=/dev/zero of=disk.img bs=1M count=64
	mkfs.vfat disk.img
	mmd -i disk.img ::/EFI
	mmd -i disk.img ::/EFI/BOOT
	mcopy -i disk.img $(EFI_APP) ::/EFI/BOOT/BOOTX64.EFI
	mcopy -i disk.img $(KERNEL_APP) ::
	mcopy -i disk.img startup.nsh ::

# --- QEMU ile Çalıştırma ---
runimg:
	qemu-system-x86_64 -bios /usr/share/ovmf/OVMF.fd -drive format=raw,file=disk.img -nographic

# --- Temizlik ---
clean:
	rm -rf $(BUILD_DIR) disk.img
