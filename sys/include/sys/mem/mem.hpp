#ifndef OOK_MEM_EARLY_INIT_HPP
#define OOK_MEM_EARLY_INIT_HPP

#include <sys/som/interface/IStream.hpp>

namespace ook::mem {
    using address_t = size_t;

    bool sane();
} // namespace ook::mem

#endif /* OOK_MEM_EARLY_INIT_HPP */
