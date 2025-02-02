#include <sys/arch/pic.hpp>

#include "sys/arch/port.hpp"

using namespace ook::arch;
using namespace ook::arch::pic;

/* static inline bool is_irq_in_pic_2(const uint8_t irq) {
    return (irq >= 8);
} */

/* static void mask_irq(uint8_t irq) {
    uint16_t port = 0;
    if (is_irq_in_pic_2(irq)) {
        port = PIC2_DATA;
        irq -= 8;
    } else {
        port = PIC1_DATA;
    }

    const uint8_t value = port::in8(port) | (1 << irq);
    port::out8(port, value);
} */

/* static void clear_irq_mask(uint8_t irq) {
    uint16_t port = 0;
    if (is_irq_in_pic_2(irq)) {
        port = PIC2_DATA;
        irq -= 8;
    } else {
        port = PIC1_DATA;
    }

    const uint8_t value = port::in8(port) & ~(1 << irq);
    port::out8(port, value);
} */

/* void pic::send_eoi(const uint8_t irq) {
    if (is_irq_in_pic_2(irq)) {
        port::out8(PIC2_COMMAND, PIC_END_INTERUPT);
    }

    port::out8(PIC1_COMMAND, PIC_END_INTERUPT);
} */

static void remap(const uint8_t offset1, const uint8_t offset2) {
    unsigned char a1, a2;

    /* Record our masks. */
    a1 = port::in8(PIC1_DATA);
    a2 = port::in8(PIC2_DATA);

    /* Cascape mode. */
    port::out8(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    port::out8(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);

    /* Master and slave PIC vector offsets. */
    port::out8(PIC1_DATA, offset1);
    port::out8(PIC2_DATA, offset2);

    /* Inform master of slave PIC (at IRQ2). */
    port::out8(PIC1_DATA, 0b00000100);

    /* Give slave PIC casade identity. */
    port::out8(PIC2_DATA, 0b00000010);

    /* TODO: unsure why we need this. */
    port::out8(PIC1_DATA, ICW4_8086);
    port::out8(PIC2_DATA, ICW4_8086);

    /* Restore our saved masks. */
    port::out8(PIC1_DATA, a1);
    port::out8(PIC2_DATA, a2);
}

void pic::init() {
    remap(240, 248);
}
