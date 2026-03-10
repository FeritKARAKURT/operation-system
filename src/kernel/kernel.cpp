#include <stdint.h>
#include "graphics/renderer.hpp"
#include "graphics/framebuffer.hpp"
#include "gdt/gdt.hpp"
#include "io.hpp"
#include "interrupt/idt.hpp"
#include "shell.hpp"
#include "drivers/keyboard.hpp"

extern void pic_remap();
Shell main_shell;
extern void shell_input_bridge(char c);

extern "C" void kernel_main(void* fb_addr, uint32_t width, uint32_t height, uint32_t pitch) {
    init_framebuffer(framebuffer, (uint64_t)fb_addr, width, height, pitch);
    renderer::clear_screen(0x000000);
    gdt::init();
    interrupt::init();
    pic_remap();

    main_shell.init();
    Keyboard::set_active_handler(shell_input_bridge);
    asm volatile("sti");
    while(1) asm volatile("hlt");
}