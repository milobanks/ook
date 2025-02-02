#ifndef OOK_ARCH_AMD64_ISR_HPP
#define OOK_ARCH_AMD64_ISR_HPP

#include <cstdint>
#include <sys/arch/idt.hpp>

extern void* const ook_arch_isr_thunks[];

namespace ook::arch::isr {
    struct InterruptFrame {
        uint16_t reg_ip;
        uint16_t reg_cs;
        uint16_t reg_flags;
        uint16_t reg_sp;
        uint16_t reg_ss;
        uint64_t error_code;
    };

    typedef idt::vector_t vector_t;
    typedef void (*func_t)(vector_t vector, const InterruptFrame* frame);

    extern func_t funcs[256];

    void register_func(vector_t vector, func_t func);
} // namespace ook::arch::isr

#endif /* OOK_ARCH_AMD64_ISR_HPP */
