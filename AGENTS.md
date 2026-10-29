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
- Optimize for human review: use lowerCamelCase for constants and plain
  `constexpr` unless shared object identity requires `inline`. Name non-obvious
  limits, explain reserved indices, and give complex predicates meaningful names.
- Keep functions focused. Prefer standalone implementation helpers for separable
  work; keep method groups and adjacent type/command declarations separated by
  blank lines. Use named types for repeated or long container declarations.
- If a function call in an `if` initializer spans multiple lines, evaluate it
  first into a descriptively named variable, then check that variable in `if`.
- Do not use output parameters. Return values directly, or use `Result<T>` for
  fallible operations; return updated state instead of mutating a caller's output
  argument. Keep required native API output parameters confined to the backend.
- Use existing `Result<T>`/`Error` patterns for fallible operations. Preserve
  resource cleanup and render-thread ownership when changing renderer code.
- Register new compiled sources and tests in the appropriate `CMakeLists.txt`.
  Add or update focused tests for behavioral changes.
- Run checks appropriate to the change. Report what passed, what was not run,
  and blockers; do not claim runtime or backend support without testing it.

## Renderer evolution constraints

- Vulkan is a target backend even while its implementation is absent. Evaluate
  renderer contracts against both Metal and Vulkan; keep native objects,
  binding locations, upload mechanics, and synchronization inside the backend.
- Define CPU message ownership, resource readiness, and GPU lifetime explicitly.
  Command acceptance, render-thread completion, and GPU completion are distinct.
- Required texture lifetime contract: releasing a texture from the caller must
  not invalidate accepted frames, including requests still pending on the CPU
  and work running on the GPU. The renderer handles safe retirement. This is
  implemented by draining accepted requests and waiting for GPU idle on release;
  preserve the guarantee if replacing that blocking path with deferred retirement.
- Prefer a mixed binding architecture using modern approaches such as bindless
  resources where feasible. Evaluate resource and shader-binding contracts on
  both Metal and Vulkan; do not force every resource through one binding model
  or describe bindless access as implemented until it exists in code.
- Agreed binding direction: renderer-managed indexed tables for sampled textures
  and reusable samplers, with explicitly bound parameter blocks for frame/draw
  data. See `docs/shader-pipeline.md` for the implemented contract and limits.
- Frame submissions must declare every texture/sampler they might access,
  including candidates selected indirectly by GPU data. Use those dependencies
  for retention and backend residency; future helpers can generate declarations.
- The user will review all code by hand. Keep the implementation and diff as
  small as possible: reuse the existing command path, add only the agreed
  capabilities, and avoid unrelated refactors or a general resource framework.
  Small scope must still preserve validation and correct resource lifetime.
- Matrix parameters use GLM `Mat4f` with explicit column-major shader compilation
  and `mul(matrix, vector)`. Preserve agreement between CPU and shader layouts;
  `ShaderParameterBlock::fromValue` copies bytes without packing or transposing.
- The asset module is due for a separate redesign, including asset ownership.
  Prioritize the renderer messaging contract over the shape of asset wrappers
  and helpers; do not make that contract depend on the current `ShaderRef` model.
- For the texture milestone, the goal is the existing triangle sampling a
  procedural checkerboard. Put procedural helpers in `asset/`; image-file
  loading is out of scope. Keep these helpers replaceable during asset redesign.
- The first texture slice is immutable 2D RGBA8 with one mip level and initial
  pixel upload at creation, plus separate reusable samplers. Texture updates,
  mip generation, and additional texture types are outside this first slice.
- Discuss extension cases before fixing a public contract, but implement only
  the agreed scope. Record new contracts in `/docs` when implemented, with
  source references; keep unimplemented proposals in the discussion.
