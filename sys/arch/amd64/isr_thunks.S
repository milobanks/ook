.intel_syntax noprefix

.extern ook_arch_isr_grand_handler

.macro push_all
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push rbp
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
.endm

.macro pop_all
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rbp
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax
.endm

/* https://stackoverflow.com/a/48170044/11657235 */

.macro isr_thunk number
    /*
     * Calls `ook_arch_isr_grand_handler`, assuming the following prototype:
     *
     *   uint64_t vector, struct InterruptFrame *frame
     */
    .align 16
    .ook_arch_isr\number:
        cld

        /*
         * The following exceptions push an error code onto the stack. This
         * makes sure the stack layout is consistent.
         */
        .if \number != 8 && \number != 10 && \number != 11 && \number != 12 && \number != 13 && \number != 14 && \number != 17 && \number != 30
            pushq 0
        .endif

        push_all
        mov rdi, \number
        mov rsi, rsp
        call ook_arch_isr_grand_handler
        pop_all
        add rsp, 8
        iretq
.endm

.altmacro

.macro irq_insert number
    # Define our thunks.
    .section .text
    isr_thunk \number

    # So the C code can load it into an array, and find it.
    .section .data
    .quad .ook_arch_isr\number
.endm

.section .data
.global ook_arch_isr_thunks
ook_arch_isr_thunks:
.set i, 0
.rept 256
    irq_insert %i
    .set i, i + 1
.endr
