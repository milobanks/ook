#include <sys/libkern/early.hpp>
#include <sys/mem/hhdm.hpp>
#include <sys/mem/mem.hpp>
#include <sys/mem/memory_map.hpp>

using namespace ook;

bool mem::sane() {
    if (memory_map_request.response == nullptr) {
        libkern::early_init_failure = "no memory map response (limine)";
        return false;
    }

    if (ook::mem::hhdm::request.response == nullptr) {
        libkern::early_init_failure = "no HHDM response (limine)";
        return false;
    }

    if (memory_map_request.response->entries == nullptr) {
        libkern::early_init_failure = "memory maps point to nowhere";
        return false;
    }

    if (memory_map_request.response->entry_count == 0) {
        libkern::early_init_failure = "no memory maps reported";
        return false;
    }

    return true;
}
