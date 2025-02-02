#include <sys/mem/MemoryMapping.hpp>

const char* ook::mem::memory_mapping_type_strings[] = {
    "usable",
    "reserved",
    "acpi reclaimable",
    "acpi sleeping",
    "bad",
    "bootloader reclaimable",
    "kernel use",
    "framebuffer",
};
