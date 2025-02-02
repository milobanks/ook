#include <sys/mem/memory_map.hpp>

__attribute__((used, section(".limine_requests"))
) volatile struct ook::boot::limine::MemmapRequest
    ook::mem::memory_map_request = {
        .id = OOK_BOOT_LIMINE_MEMMAP_ID,
        .revision = 0,
};
