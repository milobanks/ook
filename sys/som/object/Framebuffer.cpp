#include <sys/libkern/early.hpp>
#include <sys/som/object/Framebuffer.hpp>

using namespace ook::som::object;

__attribute__((used, section(".limine_requests"))
) volatile ook::boot::limine::FramebufferRequest
    ook::som::object::Framebuffer::request = {
        .id = OOK_BOOT_LIMINE_FRAMEBUFFER_ID,
        .revision = 0,
};

Framebuffer::Framebuffer(size_t which) :
    framebuffer(request.response->framebuffers[which]),
    address(framebuffer->address),
    width(framebuffer->width),
    height(framebuffer->height),
    pitch(framebuffer->pitch),
    bpp(framebuffer->bpp),
    memory_model(framebuffer->memory_model),
    red_mask_size(framebuffer->red_mask_size),
    red_mask_shift(framebuffer->red_mask_shift),
    green_mask_size(framebuffer->green_mask_size),
    green_mask_shift(framebuffer->green_mask_shift),
    blue_mask_size(framebuffer->blue_mask_size),
    blue_mask_shift(framebuffer->blue_mask_shift) {}

void* Framebuffer::get_buffer() const {
    return address;
}

size_t Framebuffer::get_size() const {
    return width * height * pitch;
}

void Framebuffer::set_pixel(size_t x, size_t y, uint8_t r, uint8_t g, uint8_t b)
    const {
    ((uint32_t*)address)[y * pitch + x] = (r << red_mask_shift)
        | (g << green_mask_shift) | (b << blue_mask_shift);
}

void Framebuffer::clear(uint8_t r, uint8_t g, uint8_t b) const {
    for (uint64_t y = 0; y < height; y++) {
        for (uint64_t x = 0; x < width; x++) {
            ((uint32_t*)address)[y * pitch + x] = (r << red_mask_shift)
                | (g << green_mask_shift) | (b << blue_mask_shift);
        }
    }
}

uint64_t Framebuffer::get_width() const {
    return width;
}

uint64_t Framebuffer::get_height() const {
    return height;
}

uint64_t Framebuffer::get_pitch() const {
    return pitch;
}

uint16_t Framebuffer::get_bpp() const {
    return bpp;
}

uint8_t Framebuffer::get_memory_model() const {
    return memory_model;
}

uint8_t Framebuffer::get_red_mask_size() const {
    return red_mask_size;
}

uint8_t Framebuffer::get_red_mask_shift() const {
    return red_mask_shift;
}

uint8_t Framebuffer::get_green_mask_size() const {
    return green_mask_size;
}

uint8_t Framebuffer::get_green_mask_shift() const {
    return green_mask_shift;
}

uint8_t Framebuffer::get_blue_mask_size() const {
    return blue_mask_size;
}

uint8_t Framebuffer::get_blue_mask_shift() const {
    return blue_mask_shift;
}

bool Framebuffer::sane() {
    if (request.response == nullptr) {
        libkern::early_init_failure = "no framebuffer response (limine)";
        return false;
    }

    if (request.response->framebuffer_count < 1) {
        libkern::early_init_failure = "no framebuffers exposed";
        return false;
    }

    return true;
}
