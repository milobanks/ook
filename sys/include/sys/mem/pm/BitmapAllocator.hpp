#ifndef OOK_MEM_PM_BITMAP_ALLOCATOR_HPP
#define OOK_MEM_PM_BITMAP_ALLOCATOR_HPP

#include <cstdint>
#include <sys/mem/PageFrameSectionTracker.hpp>

namespace ook::mem::pm {
    class BitmapAllocator {
      public:
        BitmapAllocator(PageFrameSectionTracker* memory_info);

        size_t alloc();
        size_t alloc_contiguous();
        void free_page_frame(size_t pfn);
        void free_contiguous(uint64_t pfn, size_t count);

        bool is_allocated();

      private:
        /* ... */
    };
} // namespace ook::mem::pm

#endif /* OOK_MEM_PM_BITMAP_ALLOCATOR_HPP */
