# Repository knowledge

These knowledge nodes describe the code currently in this repository:

- [Build and setup](../README.md): prerequisites and Makefile workflow.
- [Renderer design](renderer-design.md): frame flow, resources, and backend
  boundaries, grounded in the linked implementation.
- [Renderer messaging](renderer-messaging.md): payload ownership, replies,
  resource lifetime, and shutdown behavior in the current command path.
- [Shader pipeline](shader-pipeline.md): compilation, reflection, vertex layout,
  CPU materials, central asset ownership/replacement, and indexed texture/sampler binding.
- [Renderer language](CONTEXT.md): glossary for the implemented renderer concepts.
- [Implementation status](roadmap.md): source-backed renderer and platform
  status. The filename is retained from the earlier roadmap.

## Shared foundation

[kcore](../external/kcore/README.md) supplies shared utilities through the `kc`
namespace and the `kcore::kcore` CMake target. Starlight links it publicly because
engine headers use its types. [Build configuration](../CMakeLists.txt) adds the
submodule before the engine, and [Conan](../conanfile.py) provides its dependencies.
The submodule tracks `main`; both Starlight and Anchor are consumers.

[`core/Core.hh`](../src/engine/starlight/core/Core.hh) re-exports foundational
types in `sl` and exposes `kc::log` as `sl::log`. Engine callers use `log::info`,
`log::expect`, and the other logging functions through that wrapper. Other shared
utilities use kcore headers and names directly, including filesystem operations,
containers, callbacks, IDs, references, threads, and queues.

[`Error` and `Result<T>`](../src/engine/starlight/core/Error.hh) alias kcore's
types. Errors hold `std::error_code`, which distinguishes numeric values by
category. Generic errors use `kc::ErrorCode`; renderer/backend errors use
[`sl::ErrorCode`](../src/engine/starlight/core/ErrorCode.hh) and the
[`starlight` category](../src/engine/starlight/core/Error.cpp). Propagating a
kcore error preserves its category and message. Engine errors are not added to
kcore's enum.

Engine [`Config`](../src/engine/starlight/core/Config.hh),
[`EventLoop`](../src/engine/starlight/runtime/EventLoop.cpp), and the
[`event system`](../src/engine/starlight/event/EventSystem.hh) remain in Starlight.
EventLoop uses `kc::Thread` and stops and joins its worker in its destructor before
destroying scheduled calls or their mutex. The renderer executor also closes its
queue and joins its worker before teardown. Profiling uses
`KCORE_ENABLE_PROFILING`, which kcore exports to consumers when enabled.

## Maintaining knowledge nodes

Do not create ADRs (Architecture Decision Records). Update documentation inside
`/docs` as development changes the implementation. Ideas, design descriptions,
and code interface notes must reflect the repository's actual code, rather than
drive development from speculative design.

Read the implementation and callers, then update the affected node in the same
change. Link claims to source files. Remove or correct stale descriptions; do
not add features, interfaces, guarantees, or rationale that the code does not
support. A declaration, TODO, or empty stub is not evidence of working support.
If a fact cannot be verified, omit it and report the uncertainty instead.
