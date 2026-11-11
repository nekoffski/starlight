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

- `external/kcore/`: shared utilities, containers, filesystem, threading, queues,
  logging, and profiling. Both Starlight and Anchor consume kcore `main`.
- `src/engine/starlight/`: engine library. `core/` has foundational aliases,
  engine configuration and error codes; `runtime/` has the polling event loop,
  `event/` event dispatch, and `asset/` assets and loaders.
- `src/engine/starlight/renderer/`: `frontend/` handles draw preparation, requests,
  and graphs, `executor/` dispatches render-thread commands, `rhi/` defines device interfaces,
  `core/` contains resource descriptions and handles, and `shading/` stores
  validated CPU material data.
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
git submodule update --init --remote external/kcore
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
- Reuse kcore's current APIs instead of restoring duplicated utilities. Access
  logging as `log::` through `core/Core.hh`. Generic errors use `kc::ErrorCode`;
  engine/backend errors use Starlight's own `std::error_category`. Preserve
  categories when propagating errors. Ask before moving new facilities to kcore.
- Use the [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines)
  as the code-quality baseline. Use
  [GoF design patterns](https://www.informit.com/store/design-patterns-elements-of-reusable-object-oriented-9780201633610)
  to assess responsibilities and coupling; introduce a pattern only when it
  solves a concrete problem.
- Optimize for human review: use lowerCamelCase for constants and plain
  `constexpr` unless shared object identity requires `inline`. Name non-obvious
  limits, explain reserved indices, and give complex predicates meaningful names.
- Keep functions focused. Prefer standalone implementation helpers for separable
  work; keep method groups and adjacent type/command declarations separated by
  blank lines. Use named types for repeated or long container declarations.
- If a function call in an `if` initializer spans multiple lines, evaluate it
  first into a variable, then check that variable in `if`. Use `r` for status-only
  results such as `Result<void>`; use descriptive names for results carrying values.
- Do not use output parameters. Return values directly, or use `Result<T>` for
  fallible operations; return updated state instead of mutating a caller's output
  argument. Keep required native API output parameters confined to the backend.
- Prefer explicit control flow when Result combinators obscure sequencing or
  side effects. Independent asset updates must continue after an error.
- Use `log::expect` for thread and invariant checks so they remain active in
  Debug and Release builds.
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
- AssetSystem centrally stores shader/texture/sampler records behind strong references.
  Asset operations and reference lifetimes are engine-thread confined; pump
  `update()` to publish renderer futures and clean up unused records. Keep the
  shared lifecycle machinery reusable and resource-specific operations small.
  ShaderAsset/TextureAsset implement operations through an injected renderer
  proxy; SamplerAsset queues immutable sampler creation. References forward to
  their resource objects, and AssetSystem coordinates storage and lifecycle.
  Keep sampler ownership independent of textures; materials pair their handles.
  Use `waitForPending()` for blocking startup loading.
  Callers retain assets alongside non-owning renderer materials and rebuild
  captured bindings after a successful asset version change. Release references,
  finish asset shutdown, then stop rendering. Renderer messages remain independent
  of asset wrappers. See `docs/shader-pipeline.md` for the full contract.
- For the texture milestone, the goal is the existing triangle sampling a
  procedural checkerboard. Put procedural helpers in `asset/`; image-file
  loading is out of scope. Keep procedural generation independent of asset lifetime
  management.
- The first texture slice is immutable 2D RGBA8 with one mip level and initial
  pixel upload at creation, plus separate reusable samplers. Texture updates,
  mip generation, and additional texture types are outside this first slice.
- Discuss extension cases before fixing a public contract, but implement only
  the agreed scope. Record new contracts in `/docs` when implemented, with
  source references; keep unimplemented proposals in the discussion.
