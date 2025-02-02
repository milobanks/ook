# This source file is part of the Ook project (2024), which is released under
# the The GNU General Public License v3.0. You can find the license text here:
#
# 	https://www.gnu.org/licenses/gpl-3.0-standalone.html
#
# internal.pkg.libctool

$(NAME)_SOURCES := \
	drivers/uart16650.cpp \
	libkern/memory.cpp \
	libkern/early.cpp \
	libkern/memory.cpp \
	libkern/mmio.cpp \
	libkern/panic.cpp \
	libkern/printk.cpp \
	libkern/Spintex.cpp \
	libkern/String.cpp \
	mem/pm/BitmapAllocator.cpp \
	mem/hhdm.cpp \
	mem/mem.cpp \
	mem/memory_map.cpp \
	mem/MemoryMapping.cpp \
	mem/PageFrameSectionTracker.cpp \
	som/object/Framebuffer.cpp \
	som/object/UARTController.cpp \
	main.cpp

ifeq ($(ARCH), amd64)
$(NAME)_SOURCES += \
	arch/amd64/arch.cpp \
	arch/amd64/boot.S \
	arch/amd64/gdt.cpp \
	arch/amd64/hcf.cpp \
	arch/amd64/idt.cpp \
	arch/amd64/isr.cpp \
	arch/amd64/isr_thunks.cpp \
	arch/amd64/pause.cpp \
	arch/amd64/pic.cpp \
	arch/amd64/port.cpp \
	arch/amd64/uart.cpp
endif

include share/mk/ook.prog.mk
