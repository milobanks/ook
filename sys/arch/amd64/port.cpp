#include <sys/arch/port.hpp>

uint8_t ook::arch::port::in8(const port_t port) {
    uint8_t result = 0;
    asm volatile("in %1, %0\n\t" : "=a"(result) : "Nd"(port) : "memory");

    return result;
}

uint16_t ook::arch::port::in16(const port_t port) {
    uint16_t result = 0;
    asm volatile("in %1, %0" : "=a"(result) : "Nd"(port));

    return result;
}

uint32_t ook::arch::port::in32(const port_t port) {
    uint32_t result = 0;
    asm volatile("in %1, %0" : "=a"(result) : "Nd"(port));

    return result;
}

void ook::arch::port::out8(const port_t port, const uint8_t value) {
    asm volatile("out %1, %0" : : "Nd"(port), "a"(value));
}

void ook::arch::port::out16(const port_t port, const uint16_t value) {
    asm volatile("out %1, %0" : : "Nd"(port), "a"(value));
}

void ook::arch::port::out32(const port_t port, const uint32_t value) {
    asm volatile("out %1, %0" : : "Nd"(port), "a"(value));
}
