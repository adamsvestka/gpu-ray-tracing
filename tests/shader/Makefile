SUBDIR := tests/shader

include ../../Guard.mk

CXXFLAGS +=
LDFLAGS  += -lGLFW -lGLEW -framework OpenGL

OBJ_DIR = $(TEMP_DIR)/obj/$(SUBDIR)
DEP_DIR = $(TEMP_DIR)/dep/$(SUBDIR)

SRC = $(wildcard *.cpp)
OBJ = $(SRC:%.cpp=$(OBJ_DIR)/%.o)
DEP = $(SRC:%.cpp=$(DEP_DIR)/%.d)

TARGET = $(DIST_DIR)/test-shader

.DEFAULT_GOAL = build

build: $(TARGET) copy-assets

$(TARGET): $(OBJ) $(TEMP_DIR)/obj/src/common/*.o
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ): $(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	@mkdir -p $(@D:$(OBJ_DIR)%=$(DEP_DIR)%)
	$(CXX) $(CXXFLAGS) -c $< -o $@ -MMD -MF $(DEP_DIR)/$*.d

-include $(DEP)

copy-assets:
	@$(MAKE) -C glsl

.PHONY: build