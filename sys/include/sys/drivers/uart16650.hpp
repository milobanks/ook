#ifndef OOK_SYS_DRIVERS_UART16650_HPP
#define OOK_SYS_DRIVERS_UART16650_HPP

#include <cstddef>
#include <cstdint>

namespace ook::drivers::uart16650 {
    const uint8_t RBR_OFFSET = 0; // In: Receive Buffer Register
    const uint8_t THR_OFFSET = 0; // Out: Transmitter Holding Register
    const uint8_t DLL_OFFSET = 0; // Out: Divisor Latch Low
    const uint8_t IER_OFFSET = 1; // I/O: Interrupt Enable Register
    const uint8_t DLM_OFFSET = 1; // Out: Divisor Latch High
    const uint8_t FCR_OFFSET = 2; // Out: FIFO Control Register
    const uint8_t IIR_OFFSET = 2; // I/O: Interrupt Identification Register
    const uint8_t LCR_OFFSET = 3; // Out: Line Control Register
    const uint8_t MCR_OFFSET = 4; // Out: Modem Control Register
    const uint8_t LSR_OFFSET = 5; // In: Line Status Register
    const uint8_t MSR_OFFSET = 6; // In: Modem Status Register
    const uint8_t SCR_OFFSET = 7; // I/O: Scratch Register
    const uint8_t MDR1_OFFSET = 8; // I/O: Mode Register

    const uint8_t LSR_FIFOE = 0x80; // FIFO error
    const uint8_t LSR_TEMT = 0x40; // Transmitter empty
    const uint8_t LSR_THRE = 0x20; // Transmit-hold-register empty
    const uint8_t LSR_BI = 0x10; // Break interrupt indicator
    const uint8_t LSR_FE = 0x08; // Frame error indicator
    const uint8_t LSR_PE = 0x04; // Parity error indicator
    const uint8_t LSR_OE = 0x02; // Overrun error indicator
    const uint8_t LSR_DR = 0x01; // Receiver data ready
    const uint8_t LSR_BRK_ERROR_BITS = 0x1E; // BI, FE, PE, OE bits

    void initialize(
        const uint64_t base,
        const uint32_t baudrate,
        const uint32_t frequency,
        const uint8_t id
    );

    void wait_for_ready(uint64_t base);
    void transmit(uint64_t base, uint8_t c);
    void transmit(uint64_t base, const uint8_t* ptr, size_t len);
    uint8_t read(uint64_t base);
} // namespace ook::drivers::uart16650

#endif /* OOK_SYS_DRIVERS_UART16650_HPP */
