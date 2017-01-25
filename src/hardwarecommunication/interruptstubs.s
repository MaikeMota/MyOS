.set IRQ_BASE, 0x20
.section .text

.extern _ZN4MyOS21hardwarecommunication16InterruptManager15HandleInterruptEhj
.macro HandleException num
.global _ZN16InterruptManager19HandleException\num\()Ev
_ZN16InterruptManager16HandleException\num\()Ev:
    movb $\num, (interruptNumber)
    jmp int_bottom
.endm

.macro HandleInterruptRequest num
.global _ZN4MyOS21hardwarecommunication16InterruptManager26HandleInterruptRequest\num\()Ev
_ZN4MyOS21hardwarecommunication16InterruptManager26HandleInterruptRequest\num\()Ev:
    movb $\num + IRQ_BASE, (interruptNumber)
    jmp int_bottom
.endm

HandleInterruptRequest 0x00
HandleInterruptRequest 0x01
HandleInterruptRequest 0x02

int_bottom:
    # save registers
    #pusha
    #pushl %ds
    #pushl %es
    #pushl %fs
    #pushl %gs
    
    pushl %ebp
    pushl %edi
    pushl %esi

    pushl %edx
    pushl %ecx
    pushl %ebx
    pushl %eax

    # load ring 0 segment register
    #cld
    #mov $0x10, %eax
    #mov %eax, %eds
    #mov %eax, %ees

    # call C++ Handler
    pushl %esp
    push (interruptNumber)
    call _ZN4MyOS21hardwarecommunication16InterruptManager15HandleInterruptEhj # C++ Function that will handle the interruption inside the OS
    #add %esp, 6
    mov %eax, %esp # switch the stack

    # restore registers
    popl %eax
    popl %ebx
    popl %ecx
    popl %edx

    popl %esi
    popl %edi
    popl %ebp
    #pop %gs
    #pop %fs
    #pop %es
    #pop %ds
    #popa
    
    add $4, %esp

.global _ZN4MyOS21hardwarecommunication16InterruptManager15InterruptIgnoreEv
_ZN4MyOS21hardwarecommunication16InterruptManager15InterruptIgnoreEv:
    iret

.data
    interruptNumber: .byte 0
