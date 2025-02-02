#include <sys/arch/uart.hpp>

const uint64_t ook::arch::uart::ID_COM_MAPPING[] = {
    /* Reliable. */
    0x3F8,
    0x2F8,

    /* Not so reliable. */
    0x3E8,
    0x2E8,
    0x5F8,
    0x4F8,
    0x5E8,
    0x4E8,
};
