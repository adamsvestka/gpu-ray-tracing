SUBDIR := tests

include ../Guard.mk

CXXFLAGS += -I$(ROOT_DIR)/src
LDFLAGS  +=

.DEFAULT_GOAL = build

build: build-main build-shader

build-main:
	@$(MAKE) -C main

build-shader:
	@$(MAKE) -C shader

.PHONY: build build-main build-shader