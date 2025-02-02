#include <sys/arch/arch.hpp>
#include <sys/arch/hcf.hpp>
#include <sys/libkern/countof.hpp>
#include <sys/libkern/early.hpp>
#include <sys/libkern/panic.hpp>
#include <sys/libkern/printk.hpp>
#include <sys/mem/PageFrameSectionTracker.hpp>
#include <sys/mem/mem.hpp>
#include <sys/som/object/Framebuffer.hpp>
#include <sys/som/object/UARTController.hpp>

using namespace ook;
using namespace ook::som;

__attribute__((used, section(".limine_requests"))) static volatile uint64_t
    base_revision[3] = {0xf9562b2d5c95a6c8, 0x6a7b384944536bdc, 3};

static bool (*const limine_response_validity_tests[])() = {
    object::Framebuffer::sane,
    ook::mem::sane,
};

static void framebuffer_demo(interface::IFramebuffer& framebuffer) {
    uint32_t* pb_ptr = (uint32_t*)framebuffer.get_buffer();
    uint64_t width = framebuffer.get_width();
    uint64_t height = framebuffer.get_height();
    uint64_t pitch = framebuffer.get_pitch() / sizeof(uint32_t);
    uint8_t red_mask_shift = framebuffer.get_red_mask_shift();
    uint8_t green_mask_shift = framebuffer.get_green_mask_shift();
    uint8_t blue_mask_shift = framebuffer.get_blue_mask_shift();

    for (uint64_t y = 0; y < height; y++) {
        for (uint64_t x = 0; x < width; x++) {
            uint8_t red = (x * 255) / width;
            uint8_t green = (y * 255) / height;
            uint8_t blue = ((x + y) * 255) / (width + height);

            if (x > 75 && x < width - 75 && y > 75 && y < height - 75) {
                red /= 2;
                green /= 2;
                blue /= 2;
            }

            uint32_t pixel_color = (red << red_mask_shift)
                | (green << green_mask_shift) | (blue << blue_mask_shift);

            pb_ptr[y * pitch + x] = pixel_color;
        }
    }
}

static void attempt_limine_failure_communication() {
    /* Serial should basicaly always exists, we're fine. */
    const char* explanation = libkern::early_init_failure;
    ook_fatal("{s}.\n", explanation);

    /* Nothing else to do. */
    libkern::panic("an early test failed.");
}

static void reset_vt100(object::UARTController* uart) {
    uart->sendz((uint8_t*)"\033[2J\033[H");
}

static void early_test() {
    for (size_t i = 0; i < countof(limine_response_validity_tests); i++) {
        bool sane = limine_response_validity_tests[i]();

        if (!sane) {
            attempt_limine_failure_communication();
        }
    }
}

extern "C" void kernel_main() {
    /* Bring up serial ASAP. */
    object::UARTController uart(9600, 0);

    /* Tell different places where to find the UART controller. */
    libkern::panic_sink = &uart;
    libkern::printk_sink = &uart;

    /* Clear the screen and go home for VT-100 compatible terminals. */
    reset_vt100(&uart);

    /* Do some very checks to make sure we haven't gone insane. */
    early_test();

    /* We need a framebuffer, so query the first (zeroth?) one. */
    object::Framebuffer framebuffer(0);

    /* Platform specific code. */
    arch::init();

    /* Do some platform independent memory work. */
    mem::PageFrameSectionTracker page_frame_section_tracker;
    page_frame_section_tracker.show();

    /* We're okay, say ook. */
    ook_info("ook! ook? ooka ooka...\n");

    /* Pretty colors. */
    framebuffer_demo(framebuffer);

    /* Nothing else to do, for right now. */
    libkern::panic("nothing else to do.");
}
