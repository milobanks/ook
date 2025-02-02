#ifndef OOK_LIBKERN_SOM_INTERFACE_IFRAMEBUFFER_HPP
#define OOK_LIBKERN_SOM_INTERFACE_IFRAMEBUFFER_HPP

#include <cstddef>
#include <cstdint>
#include <sys/libkern/happy.hpp>
#include <sys/som/interface/IBuffer.hpp>
#include <sys/som/interface/Interface.hpp>

namespace ook::som::interface {
    class IFramebuffer: public IBuffer {
      private:
        OOK_HAPPY_CLASS;

      public:
        virtual void
        set_pixel(size_t x, size_t y, uint8_t r, uint8_t g, uint8_t b)
            const = 0;
        virtual void clear(uint8_t r, uint8_t g, uint8_t b) const = 0;
        virtual uint64_t get_width() const = 0;
        virtual uint64_t get_height() const = 0;
        virtual uint64_t get_pitch() const = 0;
        virtual uint16_t get_bpp() const = 0;
        virtual uint8_t get_memory_model() const = 0;
        virtual uint8_t get_red_mask_size() const = 0;
        virtual uint8_t get_red_mask_shift() const = 0;
        virtual uint8_t get_green_mask_size() const = 0;
        virtual uint8_t get_green_mask_shift() const = 0;
        virtual uint8_t get_blue_mask_size() const = 0;
        virtual uint8_t get_blue_mask_shift() const = 0;
    };
}; // namespace ook::som::interface

#endif /* OOK_LIBKERN_SOM_INTERFACE_IFRAMEBUFFER_HPP */
