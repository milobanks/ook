#include <sys/mem/hhdm.hpp>

__attribute__((used, section(".limine_requests"))
) volatile struct ook::boot::limine::HHDMRequest ook::mem::hhdm::request = {
    .id = OOK_BOOT_LIMINE_HHDM_ID,
    .revision = 0
};
