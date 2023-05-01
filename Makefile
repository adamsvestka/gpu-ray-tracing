export ROOT_DIR = $(CURDIR)
export IS_ROOT_MAKEFILE = 1
export SUBDIR

include Guard.mk

export CXX      = clang++
export CXXFLAGS = -std=c++20 -Wall -Wno-deprecated-volatile -I$(ROOT_DIR)/include
export LDFLAGS  =

export DIST_DIR = $(ROOT_DIR)/bin
export TEMP_DIR = $(ROOT_DIR)/tmp

# CXXFLAGS += -g -O0
# CXXFLAGS += -O3

.DEFAULT_GOAL = help

build: build-deps copy-headers build-app build-tests

build-deps: download
	@printf "\033[1;35mBuilding dependencies...\033[0m\n"
	@$(MAKE) -C lib

build-app: build-deps copy-headers copy-assets
	@printf "\033[1;35mBuilding main application binary...\033[0m\n"
	@$(MAKE) -C src

build-tests: build-deps copy-headers
	@printf "\033[1;35mBuilding tests...\033[0m\n"
	@$(MAKE) -C tests

copy-headers: download
	@printf "\033[1;33mCopying headers...\033[0m\n"
	@$(MAKE) -C include

copy-assets:
	@printf "\033[1;33mCopying assets...\033[0m\n"
	@$(MAKE) -C glsl

clean:
	@printf "\033[1;31mCleaning temporary files...\033[0m\n"
	rm -rf $(TEMP_DIR) $(DIST_DIR)

clean-all: clean
	@$(MAKE) -C lib clean
	@$(MAKE) -C include clean
	@$(MAKE) -C lib-src clean

download:
	@printf "\033[1;34mDownloading libraries...\033[0m\n"
	@$(MAKE) -C lib-src download

run: build-app
	@printf "\n"
	@printf "  \033[1;32mRunning the application...\033[0m\n"
	@printf "\n"
	@cd $(DIST_DIR) && ./main

test: test-main test-shaders

test-main: build-tests
	@printf "\n"
	@printf "  \033[1;33mRunning unit tests...\033[0m\n"
	@printf "\n"
	-@cd $(DIST_DIR) && ./test-main

test-shaders: build
	@printf "\n"
	@printf "  \033[1;33mRunning shader tests...\033[0m\n"
	@printf "\n"
	-@cd $(DIST_DIR) && ./test-shader

.PHONY: build build-app build-tests clean run test test-main test-shaders