SUBDIR := glsl

include ../Guard.mk

SRC = $(filter-out Makefile, $(wildcard */**))
TRG = $(SRC:%=$(DIST_DIR)/glsl/%)

.DEFAULT_GOAL = copy

copy: $(TRG)

$(TRG): $(DIST_DIR)/glsl/%: %
	@mkdir -p $(@D)
	cp $< $@

.PHONY: copy