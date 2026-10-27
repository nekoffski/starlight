# Working on Starlight

Starlight is a C++23 game engine built with CMake and Conan 2. Start with
[README.md](README.md) for setup and [docs/README.md](docs/README.md) for knowledge nodes.

## Documentation follows code

- Do not create ADRs (Architecture Decision Records).
- Keep project knowledge in `/docs`. Update relevant knowledge nodes during
  development, in the same change as the code they describe.
- Code is the source of truth. Architecture, design ideas, terminology, and
  interface documentation describe what exists in the repository. They evolve
  from implementation; they do not prescribe speculative implementation.
- Read the relevant code before updating a node. Link to supporting source files;
  do not invent behavior, APIs, guarantees, or rationale.
- Do not present proposals, TODOs, empty types, or backend stubs as implemented
  features. Keep future plans out of current-state knowledge nodes.
- Correct or remove stale claims when behavior changes. Update existing nodes
  instead of adding parallel design documents or decision histories.
- If something cannot be verified from the repository, leave it out and report
  the uncertainty. Do not change code just to match stale docs.

## Repository map

- `src/engine/starlight/`: engine library. `core/` has shared utilities,
  `runtime/` threading and queues, `event/` event dispatch, and `asset/` assets
  and loaders.
- `src/engine/starlight/renderer/`: `frontend/` handles requests and graphs,
  `executor/` dispatches render-thread commands, `rhi/` defines device interfaces,
  and `core/` contains resource descriptions and handles.
- `src/engine/starlight/platform/`: SDL windows and backend code. Metal has a
  device implementation; Vulkan device selection currently returns null.
- `src/sandbox/main.cpp`: engine rendering example. `src/editor/main.cpp` is
  currently a standalone SDL window/event loop.
- `src/engine/tests/`: GoogleTest tests, registered in its `CMakeLists.txt`.
- `assets/shaders/`: Slang sources; `bin/compile_shaders.sh` compiles them and
  writes adjacent reflection JSON.

## Build and checks

Run from the repository root:

```sh
make bootstrap       # Once per compiler setup; CXX can select the compiler
make build-debug     # Installs Conan dependencies, configures, builds
make build-release
make test            # Builds Debug, then runs CTest
make fmt-check       # Checks tracked C++ files using .clang-format
```

After a Debug build, `ctest --preset conan-debug --output-on-failure` reruns tests
without rebuilding. Use CTest's `-R` option to select tests by name.
`make clean` deletes `build`, `CMakeUserPresets.json`, and `.starlight`, including
its compiler profile; bootstrap again afterward.

## Change discipline

- Read affected code and callers first. Reuse existing helpers, keep changes
  focused, and preserve unrelated working-tree edits.
- Match surrounding C++ conventions: `.hh` headers, `.cpp` sources, namespace
  `sl`, and `.clang-format` (Google base, four-space indentation).
- Use existing `Result<T>`/`Error` patterns for fallible operations. Preserve
  resource cleanup and render-thread ownership when changing renderer code.
- Register new compiled sources and tests in the appropriate `CMakeLists.txt`.
  Add or update focused tests for behavioral changes.
- Run checks appropriate to the change. Report what passed, what was not run,
  and blockers; do not claim runtime or backend support without testing it.
