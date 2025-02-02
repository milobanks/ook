#ifndef OOK_MEM_VM_VM_HPP
#define OOK_MEM_VM_VM_HPP

#include <sys/mem/hhdm.hpp>

namespace ook::mem::vm {
    inline void* from_phys(uint64_t address) {
        return reinterpret_cast<void*>(
            address + hhdm::request.response->offset
        );
    }
} // namespace ook::mem::vm

#endif /* OOK_MEM_VM_VM_HPP */
