# Building Starlight

Install Python 3, Conan 2, and CMake 3.25 or newer. The C++ compiler must be
GCC 14+, Clang 18+, or Apple Clang 17+ (Xcode 16.3+).

Initialize or refresh the shared [kcore](https://github.com/nekoffski/kcore)
submodule from `main` before building:

```sh
git submodule update --init --remote external/kcore
```

CMake builds `kcore::kcore` with Starlight. Conan provides its dependencies,
including YAML and glob support. Validate changes against kcore `main` in both
Starlight and Anchor, then record the tested submodule revision.

On a new machine, select a compiler once:

```sh
make bootstrap
# Or, when the default c++ is too old:
CXX=g++-14 make bootstrap
```

Bootstrap checks compiler versions and writes an ignored Conan profile at
`.starlight/bootstrap.profile`. It does not install dependencies or build the
project. The compiler choice stays in that profile, so subsequent builds need
no `CC` or `CXX` argument:

```sh
make build-debug
# or
make build-release
```

The build target installs Conan dependencies and generates the CMake preset.
When switching compilers, run `make clean`, then bootstrap again with the new
compiler. `make clean` also removes the saved compiler profile, so bootstrap again
after cleaning.

Run `make test` to build Debug and run the GoogleTest suite through CTest.
Run `make fmt-check` to check tracked C++ files with `.clang-format`.

Debug tests include kcore's suite by default. Set `KCORE_BUILD_TESTS=OFF` when
configuring CMake to exclude it. Profiling uses kcore's option:

```sh
cmake --preset conan-debug -DKCORE_ENABLE_PROFILING=ON
```

See [repository knowledge](docs/README.md) for implementation documentation and
[AGENTS.md](AGENTS.md) for coding-agent instructions.

## Sandbox shaders

Before running the sandbox from the repository root, compile its shader assets:

```sh
bash bin/compile_shaders.sh assets/shaders
./build/Debug/src/sandbox/starlight-sandbox
```

This step needs `slangc` in `PATH` and, on macOS, the Xcode Metal Toolchain.
If `xcrun metal --version` reports a missing toolchain, install it using
`xcodebuild -downloadComponent MetalToolchain`. If lookup remains stale after
installation, run `xcrun --kill-cache` and retry `xcrun metal --version`.
Shader outputs are ignored and
must be regenerated when sources change. The sandbox samples a procedural
checkerboard through the Metal 3 indexed texture/sampler path; Vulkan rendering
is not implemented.
