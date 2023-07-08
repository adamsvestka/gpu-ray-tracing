SUBDIR := lib

include ../Guard.mk

.DEFAULT_GOAL = build

LIB_SRC = ../lib-src

build: libimgui.a

# ===== Dear ImGui =====
IMGUI_DIR = $(LIB_SRC)/imgui

IMGUI_SRCS = $(wildcard $(IMGUI_DIR)/*.cpp)
IMGUI_SRCS += $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp $(IMGUI_DIR)/backends/imgui_impl_glfw.cpp
IMGUI_OBJS = $(patsubst %.cpp,%.o,$(IMGUI_SRCS))

libimgui.a: $(IMGUI_OBJS)
	ar rcs $@ $^

$(IMGUI_OBJS): %.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ -I$(IMGUI_DIR)

clean:
	@printf "\033[1;31mCleaning library artifacts...\033[0m\n"
	rm -rf $(IMGUI_OBJS) libimgui.a libnfd.a libyaml-cpp.a

.PHONY: build clean