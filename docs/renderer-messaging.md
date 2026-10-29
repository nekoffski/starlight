# Renderer messaging

This node describes implemented code, under the
[knowledge maintenance policy](README.md#maintaining-knowledge-nodes): no ADRs,
and update these facts from code during development.

## Commands and owned payloads

[`RendererProxy`](../src/engine/starlight/renderer/RendererProxy.cpp) copies shader,
primitive, and texture upload descriptions into value commands. Commands own
bytecode, reflection, vertex/index bytes, or pixels; callers can change or free
original arrays after the proxy call. Sampler descriptions are also values.
Surface creation instead carries a shared surface provider.

Frame submission copies views, scene items, parameter-block bytes, and declared
resource dependencies. Handles in that copy do not themselves own GPU objects.

[`RendererCommand`](../src/engine/starlight/renderer/executor/RendererCommand.hh)
is a variant of payloads and promises. Submitters reference the shared
[`ThreadSafeQueue`](../src/engine/starlight/runtime/ThreadSafeQueue.hh) weakly and
move commands into it. Missing/closed queues reject submission. Queue insertion
is mutex-serialized; concurrent callers have no additional ordering protocol.

[`RenderExecutor`](../src/engine/starlight/renderer/executor/RenderExecutor.cpp)
dispatches at most one command and calls `Renderer::tick()` each loop.
[`RendererCommandDispatcher`](../src/engine/starlight/renderer/executor/RendererCommandDispatcher.cpp)
routes commands and fulfills promises. The command queue is unbounded; only the
separate pending-frame queue is bounded by `maxFramesInFlight`.

## Replies

| Operation | Result and completion boundary |
| --- | --- |
| Async shader/primitive/texture/sampler creation | `Result<future<Result<Handle>>>`: outer result means queue acceptance; future reports render-thread creation success/failure. |
| Synchronous creation | `Result<Handle>`: waits for the same creation future, rather than GPU frame completion. |
| Surface target creation | Waits for the render-thread attach operation. |
| Frame submission | Waits for acceptance/rejection by `Renderer::submit()`. Later frame errors are logged without a reply to this caller. |
| Resource destruction | Reports command acceptance; no destruction acknowledgement. Texture/sampler commands perform the blocking retirement described below. |
| `flushRenderer()` | Waits for pending frame requests to be submitted or dropped. It does not call `waitIdle()`. |

Texture creation copies pixels into a Metal texture before replying. The reply
allows subsequent ordered draw commands to use the handle; it is not a general
GPU fence. Commands rejected by a closed queue return `rendererCommandRejected`.

## Texture and sampler lifetime

[`RenderRequest::resources`](../src/engine/starlight/renderer/frontend/RenderRequest.hh)
uses `ShaderResourceDependencies` to declare every texture/sampler a frame might
access, including every possible candidate selected through GPU data. The caller must supply a complete list;
the renderer cannot infer dependencies from arbitrary parameter bytes or shader
code. Shader indices must address declared resources within reflected array
bounds; absent entries in a table snapshot are zero.

[`Renderer::submit()`](../src/engine/starlight/renderer/frontend/Renderer.cpp)
validates declared handle IDs/generations before accepting a request. A busy
frame slot leaves the oldest request pending, so FIFO command processing alone
does not guarantee that a frame is recorded before a later destroy command.

Texture/sampler destruction therefore calls `flush()`, then `waitIdle()`, then
releases the resource. Earlier accepted requests are recorded while their
resources and slots are still valid. Slot reuse happens only after those GPU
submissions finish. This stalls the render thread on release; the implementation
has no deferred retirement queue or reference-counting framework.

[`MetalResourcePool`](../src/engine/starlight/platform/metal/MetalResourcePool.cpp)
owns textures/samplers in separate 64-entry tables. Each free slot retains its
generation and increments it on destruction. A handle's ID is its logical shader
table index, independent of native argument-buffer slots. Recreated resources
can reuse indices, but old generations fail validation. Each draw's immutable
argument-table bytes prevent new resource creation from changing in-flight tables.

Other resource paths retain their existing behavior: primitive destruction
releases both device buffers but leaves the primitive map entry; shader/buffer
pool erasure is immediate; scene handles do not retain these resources; shader
destruction does not invalidate cached pipelines. The texture/sampler guarantee
does not extend to those paths.

## Shutdown

Executor stop closes the queue and joins the worker. The worker checks closure
before popping its next command, then flushes pending frames. It does not drain
remaining resource commands or explicitly fail their promises on that exit path.
Callers cannot rely on all accepted commands executing during shutdown.

[`RenderingSystem::stop()`](../src/engine/starlight/renderer/RenderingSystem.cpp)
then calls `RenderDevice::waitIdle()`. Per-slot latches in
[`MetalDevice`](../src/engine/starlight/platform/metal/MetalDevice.cpp) signal GPU
frame completion independently of proxy flush completion.
