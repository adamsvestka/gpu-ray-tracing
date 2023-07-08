SUBDIR := include

include ../Guard.mk

.DEFAULT_GOAL = copy

LIB_SRC = ../lib-src

# ===== Dear ImGui =====
IMGUI_DIR = $(LIB_SRC)/imgui
HEADERS   = $(wildcard $(IMGUI_DIR)/*.h)
HEADERS  += $(IMGUI_DIR)/backends/imgui_impl_opengl3.h $(IMGUI_DIR)/backends/imgui_impl_glfw.h

# ===== stb =====
STB_DIR  = $(LIB_SRC)/stb
HEADERS += $(STB_DIR)/stb_include.h

# ===== Doctest =====
DOCTEST_DIR = $(LIB_SRC)/doctest/doctest
HEADERS    += $(DOCTEST_DIR)/doctest.h

DIST_HEADERS  := $(notdir $(HEADERS))

copy: $(DIST_HEADERS)

define header_rule
$(1): $(2)
	mkdir -p $(dir $1) && cp $2 $1
endef

$(foreach header,$(DIST_HEADERS),\
	$(eval $(call header_rule,$(header),$(filter %/$(header),$(HEADERS)))))

clean:
	@printf "\033[1;31mCleaning library headers...\033[0m\n"
	rm -rf $(DIST_HEADERS)

.PHONY: copy clean