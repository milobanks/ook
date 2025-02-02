#ifndef OOK_LIBKERN_MEMORY_HPP
#define OOK_LIBKERN_MEMORY_HPP

#include <cstddef>

extern void* memcpy(void* dest, const void* src, size_t n);
extern void* memset(void* s, int c, size_t n);
extern void* memmove(void* dest, const void* src, size_t n);
extern int memcmp(const void* s1, const void* s2, size_t n);

namespace ook::libkern::memory {
    inline void* copy(void* dest, const void* src, size_t n) {
        return memcpy(dest, src, n);
    }

    inline void* set(void* s, int c, size_t n) {
        return memset(s, c, n);
    }

    inline void* move(void* dest, const void* src, size_t n) {
        return memmove(dest, src, n);
    }

    inline int compare(const void* s1, const void* s2, size_t n) {
        return memcmp(s1, s2, n);
    }
}; // namespace ook::libkern::memory

#endif /* OOK_LIBKERN_MEMORY_HPP */
