.PHONY: all build build-release build-debug test clean fmt fmt-check gen

CLANG_FORMAT ?= clang-format

all: build

build: build-release

build-release:
	conan install . --build=missing -s build_type=Release
	cmake --preset conan-release
	cmake --build --preset conan-release

build-debug:
	conan install . --build=missing -s build_type=Debug
	cmake --preset conan-debug
	cmake --build --preset conan-debug

fmt:
	git ls-files '*.cpp' '*.hh' '*.h' | xargs -r $(CLANG_FORMAT) -i

fmt-check:
	$(CLANG_FORMAT) --version
	git ls-files '*.cpp' '*.hh' '*.h' | xargs -r $(CLANG_FORMAT) --dry-run --Werror

test: build-debug
	ctest --preset conan-debug --output-on-failure

clean:
	rm -rf build CMakeUserPresets.json
