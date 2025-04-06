.section .text
.global _syscall
_syscall:
    // Save registers that will be used
    pushq %rdi
    pushq %rsi
    pushq %rdx
    pushq %r11
    pushq %r10
    pushq %r8
    pushq %r9

    syscall

    // Check for error
    cmpq $0, %rax
    jnl .no_error
    // If error, set errno
    movq %rax, errno
    movq $-1, %rax

.no_error:
    // Restore registers
    popq %r9
    popq %r8
    popq %r10
    popq %r11
    popq %rdx
    popq %rsi
    popq %rdi
    // Return to the caller
    ret

.section .data
errno:
    .zero 8
    .global errno
    .type errno, @object
    .size errno, 8
