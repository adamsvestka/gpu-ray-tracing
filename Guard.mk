help:
	@printf "This project uses a waterfall structure of makefiles. The makefiles in each directory are responsible\n"
	@printf "for building the files in that directory. They are called by the makefile in the root directory.\n"
	@printf "\x1b[1;33mOnly the makefile in the root directory should be called directly.\x1b[0m\n"
	@printf "\n"
ifdef SUBDIR
	@printf "You are currently in the \x1b[1;31m$(SUBDIR)\x1b[0m directory.\n"
	@printf "Run \x1b[1;32mmake -C $(shell sed -E 's|[^/]+|..|g' <<< $(SUBDIR)) \x1b[36m<target>\x1b[0m to run the target in the root directory.\n"
else
	@printf "You are currently in the \x1b[1;32mroot\x1b[0m directory.\n"
	@printf "Run \x1b[1;32mmake \x1b[36m<target>\x1b[0m to run the target.\n"
endif
	@printf "\n"
	@printf "\x1b[1;36mTargets:\x1b[0m\n"
	@printf "  \x1b[1mhelp\x1b[0m                Show this help message\n"
	@printf "  \x1b[1mbuild\x1b[0m               Build the project (default)\n"
	@printf "      \x1b[1mbuild-deps\x1b[0m      Build the dependencies\n"
	@printf "      \x1b[1mbuild-app\x1b[0m       Build the application\n"
	@printf "      \x1b[1mbuild-tests\x1b[0m     Build the tests\n"
	@printf "  \x1b[1mcopy-headers\x1b[0m        Copy the headers\n"
	@printf "  \x1b[1mcopy-assests\x1b[0m        Copy the assets\n"
	@printf "  \x1b[1mclean\x1b[0m               Clean the project\n"
	@printf "  \x1b[1mclean-all\x1b[0m           Clean the project and dependencies\n"
	@printf "  \x1b[1mdownload\x1b[0m            Download the dependencies\n"
	@printf "  \x1b[1mrun\x1b[0m                 Run the project\n"
	@printf "  \x1b[1mtest\x1b[0m                Run the tests\n"
	@printf "      \x1b[1mtest-main\x1b[0m       Run the unit tests\n"
	@printf "      \x1b[1mtest-shaders\x1b[0m    Run the shader tests\n"
	@exit 0

.PHONY: help

ifndef IS_ROOT_MAKEFILE
ifneq ($(MAKECMDGOALS),help)
$(error This makefile is not intended to be called directly. Please use the root makefile instead. Run `make help` from anywhere for more information.)
endif
endif