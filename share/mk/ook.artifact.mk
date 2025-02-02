# This source file is part of the Ook project (2024), which is released under
# the The GNU General Public License v3.0. You can find the license text here:
#
# 	https://www.gnu.org/licenses/gpl-3.0-standalone.html
#
# Deals with artifacts.

ifeq ($($(NAME)_SOURCES),)
$(error $(NAME) has no sources)
endif

$(UNIQ)_OBJS     := $(addprefix $($(UNIQ)_BUILD_DIR)/,$(addsuffix .o,$(basename $($(NAME)_SOURCES))))
$(UNIQ)_DEPFILES := $($(UNIQ)_OBJS:%.o=%.P)
$(UNIQ)_DIRS     := $(filter-out .,$(foreach DIR,$(sort $(dir $($(NAME)_SOURCES))),$(patsubst %/,%,$(DIR))))

# Assembly objects.
$(eval $(call OBJ_RULE,$($(UNIQ)_BUILD_DIR),$($(UNIQ)_SOURCE_DIR),S,$(COMPILE_S_CMDS)))
$(eval $(call OBJ_RULE,$($(UNIQ)_BUILD_DIR),$($(UNIQ)_BUILD_DIR),S,$(COMPILE_S_CMDS)))

$(foreach DIR, $($(UNIQ)_DIRS), \
		$(eval $(call OBJ_RULE,$($(UNIQ)_BUILD_DIR)/$(DIR),$($(UNIQ)_SOURCE_DIR)/$(DIR),S,$(COMPILE_S_CMDS))) \
		$(eval $(call OBJ_RULE,$($(UNIQ)_BUILD_DIR)/$(DIR),$($(UNIQ)_BUILD_DIR)/$(DIR),S,$(COMPILE_S_CMDS))) \
)

# C objects.
$(eval $(call OBJ_RULE,$($(UNIQ)_BUILD_DIR),$($(UNIQ)_SOURCE_DIR),c,$(COMPILE_C_CMDS)))
$(eval $(call OBJ_RULE,$($(UNIQ)_BUILD_DIR),$($(UNIQ)_BUILD_DIR),c,$(COMPILE_C_CMDS)))

$(foreach DIR, $($(UNIQ)_DIRS), \
	$(eval $(call OBJ_RULE,$($(UNIQ)_BUILD_DIR)/$(DIR),$($(UNIQ)_SOURCE_DIR)/$(DIR),c,$(COMPILE_C_CMDS))) \
	$(eval $(call OBJ_RULE,$($(UNIQ)_BUILD_DIR)/$(DIR),$($(UNIQ)_BUILD_DIR)/$(DIR),c,$(COMPILE_C_CMDS))) \
)

# C++ objects.
$(eval $(call OBJ_RULE,$($(UNIQ)_BUILD_DIR),$($(UNIQ)_SOURCE_DIR),cpp,$(COMPILE_CXX_CMDS)))
$(eval $(call OBJ_RULE,$($(UNIQ)_BUILD_DIR),$($(UNIQ)_BUILD_DIR),cpp,$(COMPILE_CXX_CMDS)))

$(foreach DIR, $($(UNIQ)_DIRS), \
	$(eval $(call OBJ_RULE,$($(UNIQ)_BUILD_DIR)/$(DIR),$($(UNIQ)_SOURCE_DIR)/$(DIR),cpp,$(COMPILE_CXX_CMDS))) \
	$(eval $(call OBJ_RULE,$($(UNIQ)_BUILD_DIR)/$(DIR),$($(UNIQ)_BUILD_DIR)/$(DIR),cpp,$(COMPILE_CXX_CMDS))) \
)
