#ifndef OOK_ARCH_AMD64_PIC_HPP
#define OOK_ARCH_AMD64_PIC_HPP

#include <cstdint>

namespace ook::arch::pic {
    const uint16_t PIC1 = 0x20; // IO base address for master PIC
    const uint16_t PIC2 = 0xA0; // IO base address for slave PIC

    const uint16_t PIC1_COMMAND = PIC1;
    const uint16_t PIC1_DATA = PIC1 + 1;
    const uint16_t PIC2_COMMAND = PIC2;
    const uint16_t PIC2_DATA = PIC2 + 1;

    const uint8_t PIC_END_INTERRUPT = 0x20;

    const uint8_t PIC1_CMD = 0x20;
    const uint8_t PIC2_CMD = 0xA0;
    const uint8_t PIC_READ_IRR = 0x0A; // OCW3 IRQ ready next CMD read
    const uint8_t PIC_READ_ISR = 0x0B; // OCW3 IRQ service next CMD read

    const uint8_t ICW1_ICW4 = 0x01; // ICW4 (not) needed
    const uint8_t ICW1_SINGLE = 0x02; // Single (cascade) mode
    const uint8_t ICW1_INTERVAL4 = 0x04; // Call address interval 4 (8)
    const uint8_t ICW1_LEVEL = 0x08; // Level triggered (edge) mode
    const uint8_t ICW1_INIT = 0x10; // Initialization - required!

    const uint8_t ICW4_8086 = 0x01; // 8086/88 (MCS-80/85) mode
    const uint8_t ICW4_AUTO = 0x02; // Auto (normal) EOI
    const uint8_t ICW4_BUF_SLAVE = 0x08; // Buffered mode/slave
    const uint8_t ICW4_BUF_MASTER = 0x0C; // Buffered mode/master
    const uint8_t ICW4_SFNM = 0x10; // Special fully nested (not)

    void init();
} // namespace ook::arch::pic

#endif /* OOK_ARCH_AMD64_PIC_HPP */
