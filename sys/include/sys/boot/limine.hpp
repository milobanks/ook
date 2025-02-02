/*
 * Translated from the original.
 *
 *   https://github.com/limine-bootloader/limine/blob/v8.x/limine.h
 */

#ifndef OOK_LIBKERN_LIMINE_LIMINE_HPP
#define OOK_LIBKERN_LIMINE_LIMINE_HPP

#include <cstdint>
#include <sys/libkern/platform.hpp>

namespace ook::boot::limine {
#define OOK_BOOT_LIMINE_COMMON_MAGIC 0xc7b1dd30df4c8b88, 0x0a82e883a194f07b

    struct UUID {
        uint32_t a;
        uint16_t b;
        uint16_t c;
        uint8_t d[8];
    };

    enum class MediaType {
        Generic,
        Optical,
        TrivialFileTransfer,
    };

    struct File {
        uint64_t revision;
        void* address;
        uint64_t size;
        char* path;
        char* cmdline;
        uint32_t media_type;
        uint32_t unused;
        uint32_t tftp_ip;
        uint32_t tftp_port;
        uint32_t partition_index;
        uint32_t mbr_disk_id;
        UUID gpt_disk_uuid;
        UUID gpt_part_uuid;
        UUID part_uuid;
    };

    struct BootloaderInfoResponse {
        uint64_t revision;
        char* name;
        char* version;
    };

#define OOK_BOOT_LIMINE_BOOTLOADER_INFO_ID \
    {OOK_BOOT_LIMINE_COMMON_MAGIC, 0xf55038d8e2a1202f, 0x279426fcf5f59740}

    struct BootloaderInfoRequest {
        uint64_t id[4];
        uint64_t revision;
        struct BootloaderInfoResponse* response;
    };

    enum class FirmwareType {
        X86BIOS,
        UEFI32,
        UEFI64,
        SBI,
    };

    struct FirmwareTypeResponse {
        uint64_t revision;
        uint64_t firmware_type;
    };

#define OOK_BOOT_LIMINE_FIRMWARE_TYPE_ID \
    {OOK_BOOT_LIMINE_COMMON_MAGIC, 0x8c2f75d90bef28a8, 0x7045a4688eac00c3}

    struct FirmwareTypeRequest {
        uint64_t id[4];
        uint64_t revision;
        FirmwareTypeResponse* response;
    };

    struct StackSizeResponse {
        uint64_t revision;
    };

#define OOK_BOOT_LIMINE_STACK_SIZE_ID \
    {OOK_BOOT_LIMINE_COMMON_MAGIC, 0x224ef0460a8e8926, 0xe1cb0fc25f46ea3d}

    struct StackSizeRequest {
        uint64_t id[4];
        uint64_t revision;
        StackSizeResponse* response;
        uint64_t stack_size;
    };

    struct HHDMResponse {
        uint64_t revision;
        uint64_t offset;
    };

#define OOK_BOOT_LIMINE_HHDM_ID \
    {OOK_BOOT_LIMINE_COMMON_MAGIC, 0x48dcf1cb8ad2b852, 0x63984e959a98244b}

    struct HHDMRequest {
        uint64_t id[4];
        uint64_t revision;
        HHDMResponse* response;
    };

    struct VideoMode {
        uint64_t pitch;
        uint64_t width;
        uint64_t height;
        uint16_t bpp;
        uint8_t memory_model;
        uint8_t red_mask_size;
        uint8_t red_mask_shift;
        uint8_t green_mask_size;
        uint8_t green_mask_shift;
        uint8_t blue_mask_size;
        uint8_t blue_mask_shift;
    };

    struct Framebuffer {
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
        uint8_t unused[7];
        uint64_t edid_size;
        void* edid;
        /* Response revision 1 */
        uint64_t mode_count;
        VideoMode** modes;
    };

    struct FramebufferResponse {
        uint64_t revision;
        uint64_t framebuffer_count;
        Framebuffer** framebuffers;
    };

#define OOK_BOOT_LIMINE_FRAMEBUFFER_ID \
    {OOK_BOOT_LIMINE_COMMON_MAGIC, 0x9d5827dcd881dd75, 0xa3148604f6fab11b}

    struct FramebufferRequest {
        uint64_t id[4];
        uint64_t revision;
        FramebufferResponse* response;
    };

#ifdef OOK_PLATFORM_X86_64
    const int PAGING_MODE_X86_64_4LVL = 0;
    const int PAGING_MODE_X86_64_5LVL = 1;
    const int PAGING_MODE_MIN = PAGING_MODE_X86_64_4LVL;
    const int PAGING_MODE_DEFAULT = PAGING_MODE_X86_64_4LVL;
#endif

    struct PagingModeResponse {
        uint64_t revision;
        uint64_t mode;
    };

#define OOK_BOOT_LIMINE_PAGING_MODE_ID \
    {OOK_BOOT_LIMINE_COMMON_MAGIC, 0x95c1a0edab0944cb, 0xa4e5cb3842f7488a}

    struct PagingModeRequest {
        uint64_t id[4];
        uint64_t revision;
        PagingModeResponse* response;
        uint64_t mode;
        uint64_t max_mode;
        uint64_t min_mode;
    };

    enum class MemmapType : uint64_t {
        Usable,
        Reserved,
        ACPIReclaimable,
        ACPISleeping,
        Bad,
        BootloaderReclaimable,
        ExecutableAndModules,
        Framebuffer
    };

    struct MemmapEntry {
        uint64_t base;
        uint64_t length;
        boot::limine::MemmapType type;
    };

    struct MemmapResponse {
        uint64_t revision;
        uint64_t entry_count;
        MemmapEntry** entries;
    };

#define OOK_BOOT_LIMINE_MEMMAP_ID \
    {OOK_BOOT_LIMINE_COMMON_MAGIC, 0x67cf3d9d378a806f, 0xe304acdfc50c3c62}

    struct MemmapRequest {
        uint64_t id[4];
        uint64_t revision;
        MemmapResponse* response;
    };

    typedef void (*entry_point_t)(void);

    struct EntryPointResponse {
        uint64_t revision;
    };

#define OOK_BOOT_LIMINE_ENTRY_POINT_ID \
    {OOK_BOOT_LIMINE_COMMON_MAGIC, 0x13d86c035a1cd3e1, 0x2b0caa89d8f3026a}

    struct EntryPointRequest {
        uint64_t id[4];
        uint64_t revision;
        EntryPointResponse* response;
        entry_point_t entry;
    };

    struct ExecutableFileResponse {
        uint64_t revision;
        File* executable_file;
    };

#define OOK_BOOT_LIMINE_EXECUTABLE_FILE_ID \
    {OOK_BOOT_LIMINE_COMMON_MAGIC, 0xad97e90e83f1ed67, 0x31eb5d1c5ff23b69}

    struct ExecutableFileRequest {
        uint64_t id[4];
        uint64_t revision;
        ExecutableFileResponse* response;
    };

    enum class ModuleFlags {
        Required = 1 << 0,
        Compressed = 1 << 1,
    };

    struct InternalModule {
        const char* path;
        const char* cmdline;
        uint64_t flags;
    };

    struct ModuleResponse {
        uint64_t revision;
        uint64_t module_count;
        File** modules;
    };

#define OOK_BOOT_LIMINE_MODULE_ID \
    {OOK_BOOT_LIMINE_COMMON_MAGIC, 0x3e7e279702be32af, 0xca1c4f3bd1280cee}

    struct ModuleRequest {
        uint64_t id[4];
        uint64_t revision;
        ModuleResponse* response;

        /* Request revision 1 */
        uint64_t internal_module_count;
        InternalModule** internal_modules;
    };

    struct RSDPResponse {
        uint64_t revision;
        uint64_t address;
    };

#define OOK_BOOT_LIMINE_RSDP_ID \
    {OOK_BOOT_LIMINE_COMMON_MAGIC, 0xc5e77b6b397e7b43, 0x27637845accdcf3c}

    struct RSDPRequest {
        uint64_t id[4];
        uint64_t revision;
        RSDPResponse* response;
    };

    struct SMBIOSResponse {
        uint64_t revision;
        uint64_t entry_32;
        uint64_t entry_64;
    };

#define OOK_BOOT_LIMINE_SMBIOS_ID \
    {OOK_BOOT_LIMINE_COMMON_MAGIC, 0x9e9046f11e095391, 0xaa4a520fefbde5ee}

    struct SMBIOSRequest {
        uint64_t id[4];
        uint64_t revision;
        SMBIOSResponse response;
    };

    struct EFISystemTableResponse {
        uint64_t revision;
        uint64_t address;
    };

#define OOK_BOOT_LIMINE_EFI_SYSTEM_TABLE_ID \
    {OOK_BOOT_LIMINE_COMMON_MAGIC, 0x5ceba5163eaaf6d6, 0x0a6981610cf65fcc}

    struct EFISystemTableRequest {
        uint64_t id[4];
        uint64_t revision;
        EFISystemTableResponse* response;
    };

    struct EFIMemmapResponse {
        uint64_t revision;
        void* memmap;
        uint64_t memmap_size;
        uint64_t desc_size;
        uint64_t desc_version;
    };

#define OOK_BOOT_LIMINE_EFI_MEMMAP_ID \
    {OOK_BOOT_LIMINE_COMMON_MAGIC, 0x7df62a431d6872d5, 0xa4fcdfb3e57306c8}

    struct EFIMemmapRequest {
        uint64_t id[4];
        uint64_t revision;
        EFIMemmapResponse* response;
    };

    struct BootTimeResponse {
        uint64_t revision;
        int64_t boot_time;
    };

#define OOK_BOOT_LIMINE_BOOT_TIME_ID \
    {OOK_BOOT_LIMINE_COMMON_MAGIC, 0x502746e184c088aa, 0xfbc5ec83e6327893}

    struct BootTimeRequest {
        uint64_t id[4];
        uint64_t revision;
        BootTimeResponse* response;
    };

    struct ExecutableAddressResponse {
        uint64_t revision;
        uint64_t physical_base;
        uint64_t virtual_base;
    };

#define OOK_BOOT_LIMINE_EXECUTABLE_ADDRESS_ID \
    {OOK_BOOT_LIMINE_COMMON_MAGIC, 0x71ba76863cc55f63, 0xb2644a48c516a487}

    struct ExecutableAddressRequest {
        uint64_t id[4];
        uint64_t revision;
        ExecutableAddressResponse* response;
    };

    struct DTBResponse {
        uint64_t revision;
        void* dtb_ptr;
    };

#define OOK_BOOT_LIMINE_DTB_REQUEST_ID \
    {OOK_BOOT_LIMINE_COMMON_MAGIC, 0xb40ddb48fb54bac7, 0x545081493f81ffb7}

    struct DTBRequest {
        uint64_t id[4];
        uint64_t revision;
        DTBResponse* response;
    };
} // namespace ook::boot::limine

#endif /* OOK_LIBKERN_LIMINE_LIMINE_HPP */
