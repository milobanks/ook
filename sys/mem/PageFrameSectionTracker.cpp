#include <sys/libkern/panic.hpp>
#include <sys/libkern/printk.hpp>
#include <sys/mem/PageFrameSectionTracker.hpp>
#include <sys/mem/memory_map.hpp>
#include <sys/mem/page.hpp>

#include "sys/boot/limine.hpp"

using namespace ook::mem;

PageFrameSectionTracker::PageFrameSectionTracker() {
    const struct boot::limine::MemmapResponse* const memory_map_response =
        mem::memory_map_request.response;
    struct boot::limine::MemmapEntry* const* entries =
        memory_map_response->entries;

    /*
     * Usable (and bootloader-reclaimable) mememaps are sparsely located in the
     * physical memory-space, but are stored together (contiguously) in the
     * structpage-table in virtual-memory.
     *
     * To accomplish this, assign each mememap placed in the structpage-table a
     * pfn that will be assigned to the first page residing in the mememap.
     */

    ook_debug("using page size of {D}.\n", page::SIZE);

    struct MemoryMapping memory_mappings[MAX_PAGE_FRAME_SECTIONS];
    size_t memory_mapping_count = 0;

    for (size_t i = 0; i < memory_map_response->entry_count; i++) {
        /* Sanity check. */
        if (memory_mapping_count > MAX_PAGE_FRAME_SECTIONS) {
            libkern::panic("more than MAX_MAPPINGS enties in memory map");
        }

        const struct boot::limine::MemmapEntry* const memory_map =
            memory_map_response->entries[i];

        uint64_t base = memory_map->base;
        uint64_t length = memory_map->length;

        /* Align to nearest page boundary. */
        uint64_t aligned_base = (base + (page::SIZE - 1)) & ~(page::SIZE - 1);
        uint64_t aligned_head = (base + length) & ~(page::SIZE - 1);

        /* Did alignment do something spooky? */
        if (aligned_base >= aligned_head) {
            libkern::panic("memory entry base is at or past end.");
        }

        /* Overlap or adjacency check. */
        if (memory_mapping_count != 0) {
            struct mem::MemoryMapping* prev_mapping =
                &memory_mappings[memory_mapping_count - 1];
            uint64_t prev_base = prev_mapping->base;
            uint64_t prev_head = prev_mapping->head;

            if (aligned_base < prev_head) {
                if (aligned_head <= prev_head) {
                    /* We're located entirely within the previous region, skip it */
                    continue;
                }

                /* Adjust base to remove overlap */
                aligned_base = prev_head;
            }

            /* Merge adjacent usable memory sections instead of adding new ones */
            if (memory_map->type == boot::limine::MemmapType::Usable
                && prev_mapping->type == boot::limine::MemmapType::Usable
                && prev_head == aligned_base) {
                /* Extend the previous section instead of adding a new one */
                prev_mapping->head = aligned_head;
                continue;
            }
        }

        /* Save it, yo! */
        memory_mappings[memory_mapping_count] = (struct mem::MemoryMapping) {
            .type = memory_map->type,
            .base = aligned_base,
            .head = aligned_head,
        };

        /* Map usuable sections. */
        if (memory_map->type == boot::limine::MemmapType::Usable) {
            struct mem::PageFrameSection* const section =
                &page_frame_sections[page_frame_section_count];

            /*
             * Linux calculates PFNs this way, I haven't seen any other hobby
             * operating systems that do this. Why?
             */
            section->pfn = page::pfn_from_address(memory_mappings[i].head);
            section->base = memory_mappings[memory_mapping_count].base;
            section->head = memory_mappings[memory_mapping_count].head;

            page_frame_section_count++;
        }

        memory_mapping_count++;
    }

    ook_debug(
        "found {D} page sections, {D} memory mappings.\n",
        page_frame_section_count,
        memory_mapping_count
    );
}

void PageFrameSectionTracker::show() {
    for (size_t i = 0; i != page_frame_section_count; i++) {
        const struct mem::PageFrameSection* const page_frame_section =
            &page_frame_sections[i];

        ook_debug(
            "page section: (pfn={D}) 0x{X} - 0x{X}\n",
            page_frame_section->pfn,
            page_frame_section->base,
            page_frame_section->head
        );
    }
}
