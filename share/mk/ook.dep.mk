# This source file is part of the Ook project (2024), which is released under
# the The GNU General Public License v3.0. You can find the license text here:
#
# 	https://www.gnu.org/licenses/gpl-3.0-standalone.html
#
# Handles exe-lib and lib-lib dependencies.

$(foreach lib,$($(NAME)_NEEDS_LIBS),$(if $($(lib)_ARTIFACT),,$(error can't find library $(lib))))

$(UNIQ)_NEEDS_LIBS := $(sort $($(NAME)_NEEDS_LIBS) $(foreach lib,$($(NAME)_NEEDS_LIBS),$($(lib)_NEEDS_LIBS)))
$(UNIQ)_NEEDS_LIB_INCLUDES := $(sort $(foreach lib,$($(UNIQ)_NEEDS_LIBS),$($(lib)_SOURCE_DIR)/include))
$(UNIQ)_LINKS_AGAINST := $(sort $(foreach lib,$($(UNIQ)_NEEDS_LIBS),$($(lib)_ARTIFACT)))

$(UNIQ)_INCLUDE = $($(UNIQ)_NEEDS_LIB_INCLUDES)
$(UNIQ)_DEPS = $($(UNIQ)_LINKS_AGAINST)
