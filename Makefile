# This source file is part of the Ook project (2024), which is released under
# the The GNU General Public License v3.0. You can find the license text here:
#
# 	https://www.gnu.org/licenses/gpl-3.0-standalone.html
#
# The root Makefile which includes all others.

include share/mk/contrib.gmsl.mk
include share/mk/ook.util.mk

# Makefile settings.
.SUFFIXES:
MAKEFLAGS             += --no-builtin-rules --no-builtin-variables
.DEFAULT_GOAL         := all
V                     := $(if $(VERBOSE),,@)
PACKAGES              := sys

# Package tracking.
DIRECTORY_STACK     := .
ROOT_SOURCE_DIR     := .
ROOT_SOURCE_DIR_ABS := $(CURDIR)
ROOT_BUILD_DIR      := ./build
ISO_ROOT_DIR        := $(ROOT_BUILD_DIR)/iso_root
TARGET_LIST         :=

# Cross compiling stuff.
ARCH := amd64
XCC  := /Users/milo/Software/x86_64-elf-cross/bin/x86_64-elf-gcc
XCPP := /Users/milo/Software/x86_64-elf-cross/bin/x86_64-elf-g++

# Helper rules.
.PHONY: clobber
clobber:
	$(SAY) "RM" $(CURDIR)/build
	$(V)rm -rf $(CURDIR)/build

.PHONY: db
db:
	make -nBk \
        | sed -E 's#[^ ]*/[^ ]*/*-g\+\+#clang++#g; s#[^ ]*/[^ ]*/*-gcc#clang#g' \
        | sed -E 's# -[mf][^ ]+##g' \
        | grep -w '\-c' \
        | jq -nR '[inputs|{directory:"$(ROOT_SOURCE_DIR_ABS)", command:., file: match(" [^ ]+$$").string[1:]}]' \
        > compile_commands.json

.PHONY: graph
graph:
	$(V)$(MAKE) clobber
	$(V)mkdir $(ROOT_BUILD_DIR)
	$(SAY) M2GRAPH $(ROOT_BUILD_DIR)/build.graph
	$(V)$(MAKE) -Bnd $(ACTION) | make2graph $(M2GFLAGS) > $(ROOT_BUILD_DIR)/build.graph

.PHONY: help
help:
	@printf "There are a total of $$(( $(words $(TARGET_LIST)) + 1 )) targets available.\n\n"
	@for target in $(TARGET_LIST); do \
		printf "  * $${target}\n"; \
	done
	@printf "  * all        - (build the world)\n"
	@printf "\nSee the README for more options.\n"

# Recursive package expansion.
define INCLUDE_PACKAGE
    # Build system setup.
    PACKAGES :=
    DIRECTORY_STACK := $$(call PUSH,$$(DIRECTORY_STACK),$(1))
    DISPLAY_PACKAGE := $$(subst :,.,$$(patsubst .:%,%,$$(DIRECTORY_STACK)))

    # Package setup.
    NAME := $(1)
    UNIQ := $$(call STACK_TO_VAR,$$(DIRECTORY_STACK))
    $$(UNIQ)_SOURCE_DIR := $$(call STACK_TO_PATH,$$(DIRECTORY_STACK))
    $$(UNIQ)_BUILD_DIR := $$(subst $(ROOT_SOURCE_DIR),$(ROOT_BUILD_DIR),$$($$(UNIQ)_SOURCE_DIR))

    include $$($$(UNIQ)_SOURCE_DIR)/Makefile

    $$(foreach package,$$(PACKAGES),$$(eval $$(call INCLUDE_PACKAGE,$$(package))))

    DIRECTORY_STACK := $$(call POP,$$(DIRECTORY_STACK))
endef

# And this is where it all begins...
$(foreach package,$(PACKAGES),$(eval $(call INCLUDE_PACKAGE,$(package))))

.PHONY: all
all: $(TARGET_LIST)

.PHONY: iso_root
iso_root:
	$(V)mkdir -p $(ISO_ROOT_DIR)/boot
	$(SAY) "CP" $(ISO_ROOT_DIR)/boot/vmook.elf
	$(V)cp $(ROOT_BUILD_DIR)/sys/sys $(ISO_ROOT_DIR)/boot/vmook.elf

	$(V)mkdir -p $(ISO_ROOT_DIR)/boot/limine
	$(SAY) "CP" $(ISO_ROOT_DIR)/boot/limine
	$(V)cp boot/limine.conf $(ISO_ROOT_DIR)/boot/limine

ifeq ($(ARCH),amd64)
	$(V)mkdir -p $(ISO_ROOT_DIR)/EFI/BOOT
	$(SAY) "CP" $(ISO_ROOT_DIR)/boot/limine/limine-bios.sys
	$(V)cp contrib/limine/limine-bios.sys $(ISO_ROOT_DIR)/boot/limine/
	$(SAY) "CP" $(ISO_ROOT_DIR)/boot/limine/limine-bios-cd.bin
	$(V)cp contrib/limine/limine-bios-cd.bin $(ISO_ROOT_DIR)/boot/limine/
	$(SAY) "CP" $(ISO_ROOT_DIR)/boot/limine/limine-uefi-cd.bin
	$(V)cp contrib/limine/limine-uefi-cd.bin $(ISO_ROOT_DIR)/boot/limine/
	$(SAY) "CP" $(ISO_ROOT_DIR)/EFI/BOOT/BOOTX64.EFI
	$(V)cp contrib/limine/BOOTX64.EFI $(ISO_ROOT_DIR)/EFI/BOOT/BOOTX64.EFI
	$(SAY) "CP" $(ISO_ROOT_DIR)/EFI/BOOT/BOOTIA32.EFI
	$(V)cp contrib/limine/BOOTIA32.EFI $(ISO_ROOT_DIR)/EFI/BOOT/BOOTIA32.EFI
endif

$(ROOT_BUILD_DIR)/ook.iso: $(ROOT_BUILD_DIR)/sys/sys iso_root
	$(SAY) "XORRISO" $(ROOT_SOURCE_DIR_ABS)/$@
	$(V)xorriso -as mkisofs -R -r -J -b boot/limine/limine-bios-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table -hfsplus \
		-apm-block-size 2048 --efi-boot boot/limine/limine-uefi-cd.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		$(ISO_ROOT_DIR) -o $@

.PHONY: run
run: run-$(ARCH)

.PHONY: run-amd64
run-amd64: $(ROOT_BUILD_DIR)/ook.iso
	$(SAY) qemu-system-x86_64
	$(V)qemu-system-x86_64 \
	    -machine q35 \
		-name "Ook! Dev" \
		-m 2G \
		-drive if=pflash,unit=0,format=raw,file=contrib/ovmf/ovmf-code-x86_64.fd,readonly=on \
		-cdrom $< \
		-serial mon:stdio \
		$(QEMUFLAGS)

.PHONY: debug
debug: debug-$(ARCH)

.PHONY: debug-amd64
debug-amd64:
	$(SAY) x86_64-elf-gdb
	$(V)x86_64-elf-gdb -q -ex "target remote :1234" $(ROOT_BUILD_DIR)/sys/sys

# TODO: I haven't seen a good reason why we should clean each artifact instead of just nuking
#  the build directory.
.PHONY: clean
clean:
	@printf "Consider using the 'clobber' rule instead.\n"
