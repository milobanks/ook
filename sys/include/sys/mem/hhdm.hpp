#ifndef OOK_MEM_HHDM_HPP
#define OOK_MEM_HHDM_HPP

#include <sys/boot/limine.hpp>

namespace ook::mem::hhdm {
    extern volatile struct ook::boot::limine::HHDMRequest request;
}

#endif /* OOK_MEM_HHDM_HPP */
