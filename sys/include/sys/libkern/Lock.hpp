#ifndef OOK_LIBKERN_LOCK_HPP
#define OOK_LIBKERN_LOCK_HPP

#include <cstdint>

namespace ook::libkern {
    enum LockFlags {
        Initialized = 0x01,
        Owned = 0x02,
    };

    struct Lock {
        const char* name;
        uint8_t flags;
    };
}; // namespace ook::libkern

#endif /* OOK_LIBKERN_LOCK_HPP */
