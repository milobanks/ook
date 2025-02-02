#ifndef OOK_MEM_MEMORY_INFO_HPP
#define OOK_MEM_MEMORY_INFO_HPP

#include <cstddef>
#include <sys/mem/MemoryMapping.hpp>
#include <sys/mem/PageFrameSection.hpp>

namespace ook::mem {
    constexpr size_t MAX_PAGE_FRAME_SECTIONS = 64;

    struct PageFrameSectionTracker {
        struct PageFrameSection page_frame_sections[MAX_PAGE_FRAME_SECTIONS];
        size_t page_frame_section_count;

        PageFrameSectionTracker();
        void show();
    };
} // namespace ook::mem

#endif /* OOK_MEM_MEMORY_INFO_HPP */
