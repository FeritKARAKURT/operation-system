[bits 64]
global load_idt
global isr_stub_table
extern isr_handler_c

section .text
load_idt:
    lidt [rdi]
    ret

%macro isr_stub 1
isr_stub_%1:
    push rbp
    mov rbp, rsp
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15

    mov rdi, %1
    call isr_handler_c

    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax
    pop rbp
    iretq
%endmacro

%assign i 0
%rep 256
    isr_stub i
%assign i i+1
%endrep

section .data
align 8
isr_stub_table:
%assign i 0
%rep 256
    dq isr_stub_%+i
%assign i i+1
%endrep