#ifndef OOK_MEM_PAGE_HPP
#define OOK_MEM_PAGE_HPP

#include <cstddef>
#include <cstdint>
#include <sys/arch/mem/mem.hpp>

namespace ook::mem::page {
    /* TODO: Shrink this type. */
    using pfn_t = size_t;

    const uint64_t SIZE = arch::mem::PAGE_SIZE;

    constexpr inline size_t pfn_from_address(uint64_t address) {
        return address / SIZE;
    }

    constexpr inline size_t align(uint64_t address) {
        return address & ~(SIZE - 1);
    }
}; // namespace ook::mem::page

#endif /* OOK_MEM_PAGE_HPP */
