#ifndef OOK_ARCH_AMD64_IDT_HPP
#define OOK_ARCH_AMD64_IDT_HPP

#include <cstdint>

namespace ook::arch::idt {
    const uint8_t IST_NONE = 0;
    const uint8_t INTERUPT_GATE = 0x8E;
    const uint8_t TRIP_GATE = 0x8F;

    struct GateDescriptor {
        uint16_t offset_low;
        uint16_t selector;
        uint8_t stack_table_offset;
        uint8_t flags;
        uint16_t offset_mid;
        uint32_t offset_hi;
        uint32_t reserved;
    };

    struct __attribute__((packed)) Register {
        uint16_t idt_size_minus_one;
        struct GateDescriptor* idt;
    };

    enum class ExceptionType {
        None,
        Fault,
        Trap,
        Abort,
        Interrupt,
    };

    enum class Exception {
        DivideByZero = 0,
        Debug,
        NonMaskableInterupt,
        Breakpoint,
        Overflow,
        Bound,
        InvalidOpcode,
        DeviceNotAvailable,
        DoubleFault,
        /* CoprocSegmentOverrun - deprecated */
        InvalidTSS = 10,
        SegmentNotThere,
        StackFault,
        GeneralProtectionFault,
        PageFault,
        /* 15 is unused. */
        FloatingPointFault = 16,
        AlignmentCheck,
        MachineCheck,
        SIMDFloatingPoint,
        Virtualization,
        ControlProtection,
        /* 22 - 27 are unused. */
        Hypervisor = 28,
        VirtualMemoryManager,
        Security,
        /* 31 is unused */
        TripleFault,
        NUM
    };

    typedef uint8_t vector_t;

    extern enum ExceptionType exception_types[(unsigned long)Exception::NUM];
    extern bool exception_has_error_code[(unsigned long)Exception::NUM];
    extern const char* exception_names[(unsigned long)Exception::NUM];

    extern struct GateDescriptor table[256];

    void init();
    void register_exception_handlers();
} // namespace ook::arch::idt

#endif /* OOK_ARCH_AMD64_IDT_HPP */
