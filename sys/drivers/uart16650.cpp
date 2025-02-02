#include <sys/arch/hcf.hpp>
#include <sys/arch/pause.hpp>
#include <sys/arch/port.hpp>
#include <sys/arch/uart.hpp>
#include <sys/drivers/uart16650.hpp>

using namespace ook::arch::port;
using namespace ook::drivers;

void uart16650::initialize(
    const uint64_t base,
    const uint32_t baudrate,
    const uint32_t frequency,
    const uint8_t id
) {
    /* Calculate the baud divison from frequency. */
    uint16_t baud_divisor = 0;

    if (baudrate != 0) {
        baud_divisor = (frequency + 8 * baudrate) / (16 * baudrate);
    }

    /* No interupts. */
    out8(base + IER_OFFSET, 0x00);

    /* Enable the divisor latch access bit. */
    out8(base + LCR_OFFSET, 0x80);

    /* Set the divisor if we need. */
    if (baud_divisor != 0) {
        out8(base + DLL_OFFSET, baud_divisor & 0xff);
        out8(base + DLM_OFFSET, (baud_divisor >> 8) & 0xff);
    }

    /* We expect 8-bits, with no parity, and one stop bit. */
    out8(base + LCR_OFFSET, 0x03);

    /* FIFO. */
    out8(base + FCR_OFFSET, 0x01);

    /* No modem control DTR RTS. */
    out8(base + MCR_OFFSET, 0x00);

    /* Scratchpad. */
    out8(base + SCR_OFFSET, 0x00);

    /* Clear line status, for a clean slate. */
    in8(base + LSR_OFFSET);

    /* Read the receive buffer. */
    in8(base + RBR_OFFSET);
}

void uart16650::wait_for_ready(uint64_t base) {
    /* No timeout? Wait forever. */
    while (!(in8(base + LSR_OFFSET) & LSR_THRE)) {
        ook::arch::pause::pause();
    }
}

void uart16650::transmit(uint64_t base, uint8_t c) {
    wait_for_ready(base);
    out8(base + THR_OFFSET, c);
}

void uart16650::transmit(uint64_t base, const uint8_t* ptr, size_t len) {
    for (size_t i = 0; i < len; i++) {
        transmit(base, *(ptr + i));
    }
}

uint8_t uart16650::read(uint64_t base) {
    return in8(base + RBR_OFFSET);
}
