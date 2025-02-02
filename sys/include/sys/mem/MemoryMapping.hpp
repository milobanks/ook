#ifndef OOK_MEM_MEMORY_MAPING_HPP
#define OOK_MEM_MEMORY_MAPING_HPP

#include <cstdint>
#include <sys/boot/limine.hpp>

namespace ook::mem {
    extern const char* memory_mapping_type_strings[];

    struct MemoryMapping {
        ook::boot::limine::MemmapType type;
        uint64_t base;
        uint64_t head;
    };

    const char* memory_mapping_type_string(enum boot::limine::MemmapType type);
}; // namespace ook::mem

#endif /* OOK_MEM_MEMORY_MAPING_HPP */
