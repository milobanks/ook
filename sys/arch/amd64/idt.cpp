#include <sys/arch/gdt.hpp>
#include <sys/arch/idt.hpp>
#include <sys/arch/isr.hpp>
#include <sys/libkern/countof.hpp>

using namespace ook::arch::idt;
using namespace ook::arch;

enum ExceptionType idt::exception_types[] = {
    ExceptionType::Fault, ExceptionType::Trap,  ExceptionType::Interrupt,
    ExceptionType::Trap,  ExceptionType::Trap,  ExceptionType::Fault,
    ExceptionType::Fault, ExceptionType::Fault, ExceptionType::Abort,
    ExceptionType::Fault, ExceptionType::Fault, ExceptionType::Fault,
    ExceptionType::Fault, ExceptionType::Fault, ExceptionType::Fault,
    ExceptionType::None,  ExceptionType::Fault, ExceptionType::Fault,
    ExceptionType::Abort, ExceptionType::Fault, ExceptionType::Fault,
    ExceptionType::Fault, ExceptionType::None,  ExceptionType::None,
    ExceptionType::None,  ExceptionType::None,  ExceptionType::None,
    ExceptionType::Fault, ExceptionType::Fault, ExceptionType::Fault,
    ExceptionType::None,  ExceptionType::Abort
};

bool idt::exception_has_error_code[] = {
    false, false, false, false, false, false, false, false, true,  false,
    true,  true,  true,  true,  true,  false, false, true,  false, false,
    false, true,  false, false, true,  true,  false, false, false,
};

const char* idt::exception_names[] = {
    "division error",
    "debug",
    "non-maskable interrupt",
    "breakpoint",
    "overflow",
    "bound range exceeded",
    "invalid opcode",
    "device not available",
    "double fault",
    "<copressor segment overrun>",
    "invalid TTS",
    "segment not present",
    "stack-segmentation fault",
    "general protection fault",
    "page fault",
    "<reserved>",
    "x87 floating-point",
    "alignment check",
    "machine check",
    "SIMD floating-point",
    "virtualization",
    "control protection",
    "<reserved>",
    "<reserved>",
    "<reserved>",
    "<reserved>",
    "<reserved>",
    "hypervisor injection",
    "VMM communication",
    "security exception",
    "<reserved>",
    "triple fault",
};

struct GateDescriptor idt::table[256] = {0};

static void set_vector(
    const vector_t vector,
    const uint8_t stack_table_offset,
    const uint8_t flags
) {
    const uint64_t handler_addr = (uint64_t)ook_arch_isr_thunks[vector];
    idt::table[vector] = (struct GateDescriptor) {
        .offset_low = (uint16_t)handler_addr,
        .selector = gdt::get_kernel_mode_segment(),
        .stack_table_offset = stack_table_offset,
        .flags = flags,
        .offset_mid = (uint16_t)(handler_addr >> 16),
        .offset_hi = (uint32_t)(handler_addr >> 32),
        .reserved = 0,
    };
}

static void load() {
    static struct Register reg = {
        .idt_size_minus_one = sizeof(idt::table) - 1,
        .idt = idt::table
    };

    asm volatile("lidt %0" ::"m"(reg) : "memory");
}

void idt::init() {
    for (vector_t vector = 0; vector < countof(idt::table) - 1; vector++) {
        isr::register_func(vector, nullptr);
        set_vector(vector, IST_NONE, INTERUPT_GATE);
    }

    load();
}
