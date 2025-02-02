#ifndef OOK_ARCH_AMD64_MEM_H
#define OOK_ARCH_AMD64_MEM_H

#include <cstddef>
#include <cstdint>

namespace ook::arch::mem {
    typedef uint64_t pte_t;

    struct LargepageLevelInfo {
        uint8_t order;
        uint8_t largepage_order;
        uint8_t level;

        bool is_supported : 1;
        uint64_t size;
    };

    enum class PageTableEntryFlags : uint64_t {
        Present = 1ull << 0,
        Write = 1ull << 1,
        User = 1ull << 2,
        WriteThrough = 1ull << 3,
        CacheDisabled = 1ull << 4,
        Accessed = 1ull << 5,
        // Only valid on PML1 pages and large pages
        Dirty = 1ull << 6,
        // Not valid on PML1 pages
        Large = 1ull << 7,
        Global = 1ull << 8,
        NoExec = 1ull << 63
    };

    const uint64_t PML1_SHIFT = 12;
    const uint64_t PML2_SHIFT = 21;
    const uint64_t PML3_SHIFT = 30;
    const uint64_t PML4_SHIFT = 39;
    const uint64_t PML5_SHIFT = 48;

    const uint64_t PAGE_SHIFT = PML1_SHIFT;
    const uint64_t PTE_PHYS_MASK = 0x0000fffffffff000;

    const uint64_t PGT_LEVEL_COUNT = 5;
    const uint64_t PGT_PTE_COUNT = 512;

    const uint64_t PML1_MASK = 0x1ff;
    const uint64_t PML2_MASK = PML1_MASK;
    const uint64_t PML3_MASK = PML1_MASK;
    const uint64_t PML4_MASK = PML1_MASK;
    const uint64_t PML5_MASK = PML1_MASK;

    extern struct largepage_level_info
        largepage_level_info_list[PGT_LEVEL_COUNT];

    const uint64_t PAGE_SIZE_2MIB = 1ull << PML2_SHIFT;
    const uint64_t PAGE_SIZE_1GIB = 1ull << PML3_SHIFT;

    const uint64_t LARGEPAGE_LEVEL_2MIB = 2;
    const uint64_t LARGEPAGE_LEVEL_1GIB = 3;

    const uint64_t PAGE_SIZE = 1ull << PAGE_SHIFT;

    static const uint16_t PT_LEVEL_MASKS[PGT_LEVEL_COUNT + 1] =
        {(1ull << 12) - 1, PML1_MASK, PML2_MASK, PML3_MASK, PML4_MASK, PML5_MASK
    };

    static const uint8_t PAGE_SHIFTS[PGT_LEVEL_COUNT] =
        {PML1_SHIFT, PML2_SHIFT, PML3_SHIFT, PML4_SHIFT, PML5_SHIFT};

    static const uint8_t OOK_MEM_LARGEPAGE_LEVELS[] = {2, 3};
    static const uint8_t OOK_MEM_LARGEPAGE_SHIFTS[] = {PML2_SHIFT, PML3_SHIFT};

    const uint64_t PGT_FLAGS = (uint64_t)PageTableEntryFlags::Present
        | (uint64_t)ook::arch::mem::PageTableEntryFlags::Write;
    const uint64_t PTE_LARGE_FLAGS = (uint64_t)PageTableEntryFlags::Present
        | (uint64_t)ook::arch::mem::PageTableEntryFlags::Large;
    const uint64_t PTR_LEAF_FLAGS = (uint64_t)PageTableEntryFlags::Present;

    constexpr inline uint64_t pml1(uint64_t address) {
        return ((address) >> PML1_SHIFT) & PML1_MASK;
    }

    constexpr inline uint64_t pml2(uint64_t address) {
        return ((address) >> PML2_SHIFT) & PML2_MASK;
    }

    constexpr inline uint64_t pml3(uint64_t address) {
        return ((address) >> PML3_SHIFT) & PML3_MASK;
    }

    constexpr inline uint64_t pml4(uint64_t address) {
        return ((address) >> PML4_SHIFT) & PML4_MASK;
    }

    constexpr inline uint64_t pml5(uint64_t address) {
        return ((address) >> PML5_SHIFT) & PML5_MASK;
    }

    constexpr inline uint64_t page_size_at_level(size_t level) {
        const uint64_t sizes[] = {
            PAGE_SIZE,
            PAGE_SIZE_2MIB,
            PAGE_SIZE_1GIB, /* The following aren't valid largepage sizes, but keep them */
            PAGE_SIZE_1GIB * PGT_PTE_COUNT,
            PAGE_SIZE_1GIB * PGT_PTE_COUNT * PGT_PTE_COUNT
        };

        return sizes[level];
    }

    constexpr inline uint64_t pte_to_phys(pte_t ptr) {
        return ptr & PTE_PHYS_MASK;
    }

    constexpr inline pte_t phys_create_pte(uint64_t address) {
        return address;
    }
} // namespace ook::arch::mem

#endif /* OOK_ARCH_AMD64_MEM_H */
