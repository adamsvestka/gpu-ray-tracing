SUBDIR := src

include ../Guard.mk

CXXFLAGS +=
LDFLAGS  += -lGLFW -lGLEW -framework OpenGL -L$(ROOT_DIR)/lib -limgui -lnfd -framework AppKit #-lyaml-cpp

OBJ_DIR = $(TEMP_DIR)/obj/$(SUBDIR)
DEP_DIR = $(TEMP_DIR)/dep/$(SUBDIR)

SRC = $(wildcard *.cpp */*.cpp)
OBJ = $(SRC:%.cpp=$(OBJ_DIR)/%.o)
DEP = $(SRC:%.cpp=$(DEP_DIR)/%.d)

TARGET = $(DIST_DIR)/main

.DEFAULT_GOAL = build

build: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ): $(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	@mkdir -p $(@D:$(OBJ_DIR)%=$(DEP_DIR)%)
	$(CXX) $(CXXFLAGS) -c $< -o $@ -MMD -MF $(DEP_DIR)/$*.d

-include $(DEP)

.PHONY: build