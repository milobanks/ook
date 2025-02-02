# This source file is part of the Ook project (2024), which is released under
# the The GNU General Public License v3.0. You can find the license text here:
#
# 	https://www.gnu.org/licenses/gpl-3.0-standalone.html
#
# Code for compilg C/C++/ObjC/ObjC++ code and linking it.

# COMPILE_S_CMDS - Commands for assembling assembly.
define COMPILE_S_CMDS
	$(SAY) "AS" $(ROOT_SOURCE_DIR_ABS)/$$@
	$(V)mkdir -p $$(dir $$@)
	$(V)$$(strip $(XCPP) -o $$@ -c -MD $(CFLAGS) $($(UNIQ)_CFLAGS) $$($$<.CFLAGS) \
		-ffreestanding -nostdlib -mcmodel=kernel -g \
		-I$($(UNIQ)_SOURCE_DIR)/arch/$(ARCH)/include \
		-I$($(UNIQ)_SOURCE_DIR)/include \
		-I$($(UNIQ)_BUILD_DIR)/include \
		$(addprefix -I,$($(UNIQ)_INCLUDE)) $$<)
	$$(PROCESS_DEPFILE)
endef

# COMPILE_C_CMDS - Commands for compiling C source code.
define COMPILE_C_CMDS
	$(SAY) "CC" $(ROOT_SOURCE_DIR_ABS)/$$@
	$(V)mkdir -p $$(dir $$@)
	$(V)$$(strip $(XCC) -o $$@ -c -MD $(CFLAGS) $($(UNIQ)_CFLAGS) $$($$<.CFLAGS) \
		-ffreestanding -nostdlib -fno-stack-protector -fno-stack-check -fno-PIC \
		-ffunction-sections -fdata-sections -g -O0 -m64 -march=x86-64 -mno-80387 \
		-mno-mmx -mno-sse -mno-sse2 -mno-red-zone -mcmodel=kernel \
		-I$($(UNIQ)_SOURCE_DIR)/arch/$(ARCH)/include \
		-I$($(UNIQ)_SOURCE_DIR)/include \
		-I$($(UNIQ)_BUILD_DIR)/include \
		-DMODULE="$(DISPLAY_PACKAGE)" \
		$(addprefix -I,$($(UNIQ)_INCLUDE)) $$<)
	$$(PROCESS_DEPFILE)
endef

# COMPILE_CXX_CMDS - Commands for compiling C++ source code.
define COMPILE_CXX_CMDS
	$(SAY) "CXX" $(ROOT_SOURCE_DIR_ABS)/$$@
	$(V)mkdir -p $$(dir $$@)
	$(V)$$(strip $$(XCPP) -o $$@ -c -MD $(CXXFLAGS) $($(UNIQ)_CXXFLAGS) $$($$<.CXXFLAGS) \
		-ffreestanding -nostdlib -fno-exceptions -fno-rtti -std=c++20 \
		-fno-stack-protector -fno-stack-check -fno-PIC -ffunction-sections \
		-fdata-sections -g -O0 -m64 -march=x86-64 -mno-80387 -mno-mmx -mno-sse \
		-mno-sse2 -mno-red-zone -mcmodel=kernel \
		-I$($(UNIQ)_SOURCE_DIR)/arch/$(ARCH)/include \
		-I$($(UNIQ)_SOURCE_DIR)/include \
		-I$($(UNIQ)_BUILD_DIR)/include \
		-DMODULE="\"$(DISPLAY_PACKAGE)\"" \
		$(addprefix -I,$($(UNIQ)_INCLUDE)) $$<)
	$$(PROCESS_DEPFILE)
endef

# PROCESS_DEPFILE - Process a .d file into a .P file.
define PROCESS_DEPFILE
	$(V)cp $(@:%$(suffix $@)=%.d) $(@:%$(suffix $@)=%.P); \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
	    -e '/^$$/ d' -e 's/$$/ :/' < $(@:%$(suffix $@)=%.d) \
	    >> $(@:%$(suffix $@)=%.P); \
	  rm -f $(@:%$(suffix $@)=%.d)
endef

# DEFINE_OBJ_RULE - Define a single object building rule.
define OBJ_RULE
$(1)/%.o: override NAME := $$(NAME)
$(1)/%.o: $(2)/%.$(3)
	$(4)
endef
