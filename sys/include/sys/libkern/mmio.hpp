#ifndef OOK_ARCH_MMIO_HPP
#define OOK_ARCH_MMIO_HPP

#include <cstdint>

namespace ook::arch::mmio {
    uint8_t read8(volatile const uint8_t* const ptr);
    uint16_t read16(volatile const uint8_t* const ptr);
    uint32_t read32(volatile const uint8_t* const ptr);
    uint64_t read64(volatile const uint8_t* const ptr);
    void write8(volatile uint8_t* const ptr, const uint8_t value);
    void write16(volatile uint8_t* const ptr, const uint16_t value);
    void write32(volatile uint8_t* const ptr, const uint32_t value);
    void write64(volatile uint8_t* const ptr, const uint64_t value);
} // namespace ook::arch::mmio

#endif /* OOK_ARCH_PAUSE_HPP */
