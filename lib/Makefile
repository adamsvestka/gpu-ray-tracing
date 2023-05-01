SUBDIR := lib

include ../Guard.mk

.DEFAULT_GOAL = build

LIB_SRC = ../lib-src

build: libimgui.a libnfd.a

# ===== Dear ImGui =====
IMGUI_DIR = $(LIB_SRC)/imgui

IMGUI_SRCS = $(wildcard $(IMGUI_DIR)/*.cpp)
IMGUI_SRCS += $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp $(IMGUI_DIR)/backends/imgui_impl_glfw.cpp
IMGUI_OBJS = $(patsubst %.cpp,%.o,$(IMGUI_SRCS))

libimgui.a: $(IMGUI_OBJS)
	ar rcs $@ $^

$(IMGUI_OBJS): %.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ -I$(IMGUI_DIR)

# ===== Native File Dialog =====
NFD_DIR = $(LIB_SRC)/nativefiledialog/build

NFD_TARGETS = $(wildcard $(NFD_DIR)/gmake_*)
NFD_OBJS = $(shell find $(NFD_DIR) -type d -name 'obj')
NFD_OBJS += $(NFD_DIR)/lib $(NFD_DIR)/bin

libnfd.a: $(NFD_TARGETS)
	@BUILD_SUCCESS=false; \
	for target in $(NFD_TARGETS); do \
		make -C $$target && BUILD_SUCCESS=true || true; \
	done; \
	if [ "$$BUILD_SUCCESS" = "false" ]; then \
		echo "All builds failed!"; \
		exit 1; \
	fi
	find $(NFD_DIR)/lib -type f -name '*.a' -exec cp {} . \;

clean:
	@printf "\033[1;31mCleaning library artifacts...\033[0m\n"
	rm -rf $(IMGUI_OBJS) $(NFD_OBJS) libimgui.a libnfd.a

.PHONY: build clean