#ifndef OOK_MEM_MEMORY_MAP_HPP
#define OOK_MEM_MEMORY_MAP_HPP

#include <sys/boot/limine.hpp>

namespace ook::mem {
    extern volatile struct ook::boot::limine::MemmapRequest memory_map_request;
}

#endif /* OOK_MEM_MEMORY_MAP_HPP */
