.PHONY: all bootstrap require-bootstrap build build-release build-debug test clean fmt fmt-check gen

CLANG_FORMAT ?= clang-format
BOOTSTRAP_PROFILE := .starlight/bootstrap.profile

all: build

build: build-release

bootstrap:
	python3 bin/bootstrap.py

require-bootstrap:
	@test -f $(BOOTSTRAP_PROFILE) || { echo "Run make bootstrap first (or CXX=/path/to/compiler make bootstrap)" >&2; exit 1; }

build-release: require-bootstrap
	conan install . --build=missing -pr:a $(BOOTSTRAP_PROFILE) -s build_type=Release
	cmake --preset conan-release
	cmake --build --preset conan-release

build-debug: require-bootstrap
	conan install . --build=missing -pr:a $(BOOTSTRAP_PROFILE) -s build_type=Debug
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
	rm -rf build CMakeUserPresets.json .starlight
