#include <sys/arch/gdt.hpp>

using namespace ook::arch::gdt;
using namespace ook::arch;

/* https://wiki.osdev.org/GDT_Tutorial#Flat_/_Long_Mode_Setup */
struct GDT gdt::gdt = {
    .entries =
        {
            {0},
            {
                .limit = 0xFFFF,
                .base_low = 0,
                .base_mid = 0,
                .access = ACC_KERNEL_CODE | ACC_PRESENT,
                .granularity = 0,
                .base_high = 0,
            },
            {
                .limit = 0xFFFF,
                .base_low = 0,
                .base_mid = 0,
                .access = ACC_KERNEL_DATA | ACC_PRESENT,
                .granularity = 0,
                .base_high = 0,
            },
            {
                .limit = 0xFFFF,
                .base_low = 0,
                .base_mid = 0,
                .access = ACC_KERNEL_CODE | ACC_PRESENT,
                .granularity = GRANULARITY_32,
                .base_high = 0,
            },
            {
                .limit = 0xFFFF,
                .base_low = 0,
                .base_mid = 0,
                .access = ACC_KERNEL_DATA | ACC_PRESENT,
                .granularity = GRANULARITY_32,
                .base_high = 0,
            },
            {
                .limit = 0,
                .base_low = 0,
                .base_mid = 0,
                .access = ACC_KERNEL_CODE | ACC_PRESENT,
                .granularity = GRANULARITY_64,
                .base_high = 0,
            },
            {
                .limit = 0,
                .base_low = 0,
                .base_mid = 0,
                .access = ACC_KERNEL_DATA | ACC_PRESENT,
                .granularity = GRANULARITY_64,
                .base_high = 0,
            },
            {0},
            {0},
            {
                .limit = 0,
                .base_low = 0,
                .base_mid = 0,
                .access = ACC_USER_CODE | ACC_PRESENT,
                .granularity = GRANULARITY_64,
                .base_high = 0,
            },
            {
                .limit = 0,
                .base_low = 0,
                .base_mid = 0,
                .access = ACC_USER_DATA | ACC_PRESENT,
                .granularity = GRANULARITY_64,
                .base_high = 0,
            },
        },
    .tss =
        {
            .length = 104,
            .base_low = 0,
            .base_mid = 0,
            .flags1 = TSS_64 | TSS_PRESENT,
            .base_high = 0,
            .base_upper = 0,
            .reserved = 0,
        },
};

void gdt::init() {
    /* Prelude. */
    asm volatile("cli");

    /* Load the GDT. */
    asm volatile("lgdt (%0)" : : "r"(&gdt_pointer) : "memory");

    /* Switch via far return. */
    asm volatile(
        "push $0x28\n"
        "lea 1f(%rip), %rax\n"
        "push %rax\n"
        "lretq\n"
        "1:\n"
    );

    /* Load the data segment. */
    asm volatile(
        "mov $0x30, %eax\n"
        "mov %eax, %ds\n"
        "mov %eax, %es\n"
        "mov %eax, %fs\n"
        "mov %eax, %gs\n"
        "mov %eax, %ss\n"
    );

    /* Postlude. */
    asm volatile("sti");
}

uint16_t gdt::get_kernel_mode_segment() {
    return KERNEL_CODE_64_INDEX * sizeof(struct GDTDescriptor);
}

uint16_t gdt::get_user_mode_segment() {
    return USER_DATA_64_INDEX * sizeof(struct GDTDescriptor);
}
