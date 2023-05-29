SUBDIR := lib

include ../Guard.mk

.DEFAULT_GOAL = build

LIB_SRC = ../lib-src

build: libimgui.a libnfd.a #libyaml-cpp.a

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

ifeq ($(shell uname -s), Darwin)
	NFD_TARGET = $(NFD_DIR)/gmake_macosx
else ifeq ($(shell uname -s), Linux)
	NFD_TARGET = $(NFD_DIR)/gmake_linux
else ifeq ($(shell uname -s), Windows_NT)
	NFD_TARGET = $(NFD_DIR)/gmake_windows
else
	$(error Unsupported platform)
endif

NFD_OBJS = $(shell find $(NFD_DIR) -type d -name 'obj')
NFD_OBJS += $(NFD_DIR)/lib $(NFD_DIR)/bin

libnfd.a: $(NFD_TARGET)
	$(MAKE) -C $(NFD_TARGET)
	find $(NFD_DIR)/lib -type f -name $@ -exec cp {} . \;

# ===== YAML CPP =====
YAML_CPP_DIR = $(LIB_SRC)/yaml-cpp

libyaml-cpp.a:
	cmake -S $(YAML_CPP_DIR) -B $(YAML_CPP_DIR)/build -DYAML_CPP_BUILD_TESTS=OFF -DYAML_CPP_BUILD_TOOLS=OFF
	$(MAKE) -C $(YAML_CPP_DIR)/build
	cp $(YAML_CPP_DIR)/build/$@ .

YAML_CPP_OBJS = $(YAML_CPP_DIR)/build

clean:
	@printf "\033[1;31mCleaning library artifacts...\033[0m\n"
	rm -rf $(IMGUI_OBJS) $(NFD_OBJS) $(YAML_CPP_OBJS) libimgui.a libnfd.a libyaml-cpp.a

.PHONY: build clean