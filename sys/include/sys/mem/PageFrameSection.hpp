#ifndef OOK_MEM_PAGE_SECTION_HPP
#define OOK_MEM_PAGE_SECTION_HPP

#include <cstdint>

namespace ook::mem {
    struct PageFrameSection {
        uint64_t base;
        uint64_t head;
        uint64_t pfn;
    };
}; // namespace ook::mem

#endif /* OOK_MEM_PAGE_SECTION_HPP */
