SUBDIR := res

include ../Guard.mk

SRC = $(filter-out Makefile, $(wildcard *))
TRG = $(SRC:%=$(DIST_DIR)/%)

.DEFAULT_GOAL = copy

copy: #$(TRG)

$(TRG): $(DIST_DIR)/%: %
	@mkdir -p $(@D)
	cp $< $@

.PHONY: copy