#ifndef OOK_ARCH_AMD64_GDT_HPP
#define OOK_ARCH_AMD64_GDT_HPP

#include <cstdint>

namespace ook::arch::gdt {
    const uint8_t ACC_CODE_READABLE = (1 << 1);
    const uint8_t ACC_DATA_WRITABLE = ACC_CODE_READABLE;

    const uint8_t ACC_DATA_DOWN = (1 << 2);
    const uint8_t ACC_CODE_RING_CONFIRM = ACC_DATA_DOWN;

    const uint8_t ACC_CODE_SEG = (1 << 3);
    const uint8_t ACC_NON_TSS_SEG = (1 << 4);

    const uint8_t ACC_DPL3 = (3 << 5);
    const uint8_t ACC_PRESENT = (1 << 7);

    const uint8_t ACC_KERNEL_CODE =
        (ACC_CODE_SEG | ACC_CODE_READABLE | ACC_NON_TSS_SEG);
    const uint8_t ACC_KERNEL_DATA = (ACC_DATA_WRITABLE | ACC_NON_TSS_SEG);

    const uint8_t ACC_USER_CODE =
        (ACC_CODE_SEG | ACC_CODE_READABLE | ACC_NON_TSS_SEG | ACC_DPL3);
    const uint8_t ACC_USER_DATA =
        (ACC_DATA_WRITABLE | ACC_NON_TSS_SEG | ACC_DPL3);

    const uint8_t GRANULARITY_LIMIT_HIGH = 0b10001111;
    const uint8_t GRANULARITY_LONG = (1 << 5);
    const uint8_t GRANULARITY_32_BIT_SEG = (1 << 6);

    const uint8_t GRANULARITY_32 =
        (GRANULARITY_32_BIT_SEG | GRANULARITY_LIMIT_HIGH);
    const uint8_t GRANULARITY_64 = GRANULARITY_LONG;

    const uint8_t TSS_64 = 0b1001;
    const uint8_t TSS_PRESENT = 0b10000000;

    const uint8_t NULL_INDEX = 0;

    const uint8_t KERNEL_CODE_16_INDEX = 1;
    const uint8_t KERNEL_DATA_16_INDEX = 2;
    const uint8_t KERNEL_CODE_32_INDEX = 3;
    const uint8_t KERNEL_DATA_32_INDEX = 4;
    const uint8_t KERNEL_CODE_64_INDEX = 5;
    const uint8_t KERNEL_DATA_64_INDEX = 6;
    const uint8_t USER_CODE_64_INDEX = 9;
    const uint8_t USER_DATA_64_INDEX = 10;

    struct GDTDescriptor {
        uint16_t limit;
        uint16_t base_low;
        uint8_t base_mid;
        uint8_t access;
        uint8_t granularity;
        uint8_t base_high;
    };

    struct TSSDescriptor {
        uint16_t length;
        uint16_t base_low;
        uint8_t base_mid;
        uint8_t flags1;
        uint8_t flags2;
        uint8_t base_high;
        uint32_t base_upper;
        uint32_t reserved;
    };

    struct GDT {
        struct GDTDescriptor entries[11];
        struct TSSDescriptor tss;
    };

    extern struct GDT gdt;

    struct __attribute__((packed)) GDTPointer {
        uint16_t size;
        struct GDT* gdt;
    };

    static struct GDTPointer gdt_pointer = {
        .size = sizeof(gdt) - 1,
        .gdt = &gdt
    };

    void init();

    uint16_t get_kernel_mode_segment();
    uint16_t get_user_mode_segment();
} // namespace ook::arch::gdt

#endif /* OOK_ARCH_AMD64_GDT_HPP */
