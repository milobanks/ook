#ifndef OOK_LIBKERN_SLICE_HPP
#define OOK_LIBKERN_SLICE_HPP

#include <cstddef>

namespace ook::libkern {
    template<typename T>
    struct Slice {
        size_t length;
        const T* data;
    };
}; // namespace ook::libkern

#endif /* OOK_LIBKERN_SLICE_HPP */
