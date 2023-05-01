SUBDIR := lib-src

include ../Guard.mk

LIBRARIES = $(filter-out Makefile,$(wildcard *))

.DEFAULT_GOAL = download

download: $(patsubst %,%/.git,$(LIBRARIES))

define library_rule
$(1)/.git:
	@printf "\033[1;34mDownloading $(1)...\033[0m\n"
	git submodule update --init --recursive $(1)
endef

$(foreach lib,$(LIBRARIES),$(eval $(call library_rule,$(lib))))

clean:
	@printf "\033[1;31mCleaning libraries...\033[0m\n"
	rm -rf $(foreach lib,$(LIBRARIES),$(lib)/{,.[^.],..?}*)

.PHONY: download clean