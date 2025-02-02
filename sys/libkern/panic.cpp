#include <sys/arch/hcf.hpp>
#include <sys/libkern/panic.hpp>

using namespace ook;

som::interface::IStream<uint8_t>* libkern::panic_sink;

static void try_write_sink(const char* msg) {
    libkern::panic_sink->sendz(reinterpret_cast<const uint8_t*>("*** "));
    libkern::panic_sink->sendz(reinterpret_cast<const uint8_t*>(msg));
    libkern::panic_sink->sendz(reinterpret_cast<const uint8_t*>(" ***\n\a"));
}

void libkern::panic(const char* msg) {
    if (panic_sink) {
        try_write_sink(msg);
    }

    arch::hcf::hcf();
}
