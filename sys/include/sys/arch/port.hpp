#ifndef OOK_ARCH_PORT_HPP
#define OOK_ARCH_PORT_HPP

#include <cstdint>

namespace ook::arch::port {
    using port_t = uint16_t;

    uint8_t in8(const port_t port);
    uint16_t in16(const port_t port);
    uint32_t in32(const uint16_t port);
    void out8(const port_t port, const uint8_t value);
    void out16(const port_t port, const uint16_t value);
    void out32(const port_t port, const uint32_t value);
} // namespace ook::arch::port

#endif /* OOK_ARCH_PORT_HPP */
