#include <sys/libkern/mmio.hpp>

uint8_t ook::arch::mmio::read8(volatile const uint8_t* const ptr) {
    return *(volatile const uint8_t*)ptr;
}

uint16_t ook::arch::mmio::read16(volatile const uint8_t* const ptr) {
    return *(volatile const uint16_t*)ptr;
}

uint32_t ook::arch::mmio::read32(volatile const uint8_t* const ptr) {
    return *(volatile const uint32_t*)ptr;
}

uint64_t ook::arch::mmio::read64(volatile const uint8_t* const ptr) {
    return *(volatile const uint64_t*)ptr;
}

void ook::arch::mmio::write8(volatile uint8_t* const ptr, const uint8_t value) {
    *(volatile uint8_t*)ptr = value;
}

void ook::arch::mmio::write16(
    volatile uint8_t* const ptr,
    const uint16_t value
) {
    *(volatile uint16_t*)ptr = value;
}

void ook::arch::mmio::write32(
    volatile uint8_t* const ptr,
    const uint32_t value
) {
    *(volatile uint32_t*)ptr = value;
}

void ook::arch::mmio::write64(
    volatile uint8_t* const ptr,
    const uint64_t value
) {
    *(volatile uint64_t*)ptr = value;
}
