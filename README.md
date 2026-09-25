# Building Starlight

Install Python 3, Conan 2, and CMake 3.23 or newer. The C++ compiler must be
GCC 14+, Clang 18+, or Apple Clang 17+ (Xcode 16.3+).

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
compiler. `make clean` keeps the saved compiler profile.
