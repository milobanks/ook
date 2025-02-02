#include <sys/arch/pause.hpp>
#include <sys/libkern/Spintex.hpp>

using namespace ook::libkern;

Spintex::Spintex(const char* name) :
    lock({.name = name, .flags = LockFlags::Initialized}) {
    std::atomic_store(&state, false);
}

void Spintex::lock_spin() {
    while (atomic_exchange(&state, true)) {
        /* Busy-wait until lock is free. */
        while (atomic_load(&state)) {
            ook::arch::pause::pause();
        }
    }

    lock.flags |= LockFlags::Owned;
}

void Spintex::unlock_spin() {
    lock.flags &= ~LockFlags::Owned;
    atomic_store(&state, false);
}
