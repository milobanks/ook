#ifndef OOK_LIBKERN_PANIC_HPP
#define OOK_LIBKERN_PANIC_HPP

#include <cstdint>
#include <sys/som/interface/IStream.hpp>

namespace ook::libkern {
    extern som::interface::IStream<uint8_t>* panic_sink;

    [[noreturn]] void panic(const char* msg);
}; // namespace ook::libkern

#endif /* OOK_LIBKERN_PANIC_HPP */
