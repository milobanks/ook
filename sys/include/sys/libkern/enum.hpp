#ifndef OOK_LIBKERN_ENUM_HPP
#define OOK_LIBKERN_ENUM_HPP

namespace ook::libkern {
    template<typename T, T First, T Last>
    constexpr int enum_size() {
        return static_cast<int>(Last) - static_cast<int>(First) + 1;
    }
}; // namespace ook::libkern

#endif /* OOK_LIBKERN_ENUM_HPP */
