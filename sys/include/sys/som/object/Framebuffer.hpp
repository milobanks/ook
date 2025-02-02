#ifndef OOK_LIBKERN_DEV_FRAMEBUFFER_HPP
#define OOK_LIBKERN_DEV_FRAMEBUFFER_HPP

#include <cstddef>
#include <cstdint>
#include <sys/boot/limine.hpp>
#include <sys/libkern/happy.hpp>
#include <sys/som/interface/IFramebuffer.hpp>

namespace ook::som::object {
    class Framebuffer: public som::interface::IFramebuffer {
      private:
        OOK_HAPPY_CLASS;

        static volatile ook::boot::limine::FramebufferRequest request;

        boot::limine::Framebuffer* framebuffer;

        void* address;
        uint64_t width;
        uint64_t height;
        uint64_t pitch;
        uint16_t bpp;
        uint8_t memory_model;
        uint8_t red_mask_size;
        uint8_t red_mask_shift;
        uint8_t green_mask_size;
        uint8_t green_mask_shift;
        uint8_t blue_mask_size;
        uint8_t blue_mask_shift;

      public:
        Framebuffer(size_t which);

        /* IBuffer */
        void* get_buffer() const override;
        size_t get_size() const override;

        /* IFramebuffer */
        void set_pixel(size_t x, size_t y, uint8_t r, uint8_t g, uint8_t b)
            const override;
        void clear(uint8_t r, uint8_t g, uint8_t b) const override;
        uint64_t get_width() const override;
        uint64_t get_height() const override;
        uint64_t get_pitch() const override;
        uint16_t get_bpp() const override;
        uint8_t get_memory_model() const override;
        uint8_t get_red_mask_size() const override;
        uint8_t get_red_mask_shift() const override;
        uint8_t get_green_mask_size() const override;
        uint8_t get_green_mask_shift() const override;
        uint8_t get_blue_mask_size() const override;
        uint8_t get_blue_mask_shift() const override;

        /* Kernel support. */
        static bool sane();
    };
}; // namespace ook::som::object

#endif /* OOK_LIBKERN_DEV_FRAMEBUFFER_HPP */
