# Renderer messaging

This node describes implemented code, under the
[knowledge maintenance policy](README.md#maintaining-knowledge-nodes): no ADRs,
and update these facts from code during development.

## Commands and owned payloads

[`RendererProxy`](../src/engine/starlight/renderer/RendererProxy.cpp) copies shader,
CPU geometry, and texture upload descriptions into value commands. Primitive
creation accepts [`GeometryData`](../src/engine/starlight/geometry/GeometryData.hh)
from the CPU geometry module. Commands own
bytecode, reflection, vertex/index bytes, or pixels; callers can change or free
original arrays after the proxy call. Sampler descriptions are also values.
Surface creation instead carries a shared surface provider.

Frame submission copies views, scene items, parameter-block bytes, and declared
resource dependencies. Primitive selections include a handle and optional index
range; these values are copied too. Handles in that copy do not themselves own GPU objects.

[`RendererCommand`](../src/engine/starlight/renderer/executor/RendererCommand.hh)
is a variant of payloads and promises.
[`RendererCommandSubmitter`](../src/engine/starlight/renderer/executor/RendererCommandSubmitter.hh)
is a standalone queue endpoint returned by `RenderExecutor::createSubmitter()`.
It references the shared
[`kc::ThreadSafeQueue`](../external/kcore/include/kcore/runtime/ThreadSafeQueue.hh) weakly and
moves commands into it. Missing/closed queues reject submission. Queue insertion
is mutex-serialized; concurrent callers have no additional ordering protocol.
`RendererProxy` depends on this submitter and command data without including
the executor or frontend renderer implementation headers.

[`RenderExecutor`](../src/engine/starlight/renderer/executor/RenderExecutor.cpp)
dispatches at most one command and calls `Renderer::tick()` each loop.
[`RendererCommandDispatcher`](../src/engine/starlight/renderer/executor/RendererCommandDispatcher.cpp)
routes commands and fulfills promises through an exhaustive `std::visit()`
visitor. Its typed handlers live in the implementation; the dispatcher header
exposes only construction and `dispatch()`. Every command variant alternative
requires a handler, with missing handlers rejected at compile time.
The command queue is unbounded; only the
separate pending-frame queue is bounded by `maxFramesInFlight`.

## Replies

[`RendererProxy`](../src/engine/starlight/renderer/RendererProxy.hh) keeps typed
public operations and shares command enqueueing through private `enqueue()` and
`enqueueWithReply()` helpers. The latter obtains a command's future before
moving it into the queue. Synchronous operations use `waitForReply()` in the
implementation to propagate queue rejection or wait for the render-thread reply;
`flushRenderer()` uses the same path with its void reply.
If a synchronous reply is abandoned, `waitForReply()` catches
`std::future_error` and returns `invalidFuture`. Async creation still exposes
the raw future; its caller handles future exceptions. Asset future polling
already converts those exceptions into `invalidFuture`.

| Operation | Result and completion boundary |
| --- | --- |
| Async shader/primitive/texture/sampler creation | `Result<future<Result<Handle>>>`: outer result means queue acceptance; future reports render-thread creation success/failure. |
| Synchronous creation | `Result<Handle>`: waits for the same creation future, rather than GPU frame completion. |
| Surface target creation | Waits for the render-thread attach operation. |
| `getShaderInterface()` | Waits for a copied shader handle and ordinary block names/sizes from the render thread; invalid handles return an error. Does not retain the shader or wait for GPU completion. |
| Frame submission | Waits for acceptance/rejection by `Renderer::submit()`. Later frame errors are logged without a reply to this caller. |
| Resource destruction | Reports command acceptance; no destruction acknowledgement. Shader/texture/sampler commands perform the blocking retirement described below. |
| `flushRenderer()` | Waits for pending frame requests to be submitted or dropped. It does not call `waitIdle()`. |

Texture creation copies pixels into a Metal texture before replying. The reply
allows subsequent ordered draw commands to use the handle; it is not a general
GPU fence. Commands rejected by a closed queue return `rendererCommandRejected`.

## Shader, texture, and sampler lifetime

[`RenderRequest::resources`](../src/engine/starlight/renderer/frontend/RenderRequest.hh)
uses `ShaderResourceDependencies` to declare every texture/sampler a frame might
access, including every possible candidate selected through GPU data. The caller must supply a complete list;
the renderer cannot infer dependencies from arbitrary parameter bytes or shader
code. Shader indices must address declared resources within reflected array
bounds; absent entries in a table snapshot are zero.

For the CPU material path, callers declare dependencies alongside material and
draw parameters. The frontend's
[`RenderRequestBuilder::build()`](../src/engine/starlight/renderer/frontend/RenderRequestBuilder.cpp)
combines these with existing frame
dependencies before submission. Collection owns draw bytes and handles but
borrows materials until each build; successful builds own their snapshot data.
Raw requests still require complete manual
declarations. Handles stored by materials are non-owning, and preparation does
not prove GPU resource liveness; render-thread validation remains authoritative.

[`Renderer::submit()`](../src/engine/starlight/renderer/frontend/Renderer.cpp)
calls `validateRequest()` to check queue capacity, declared texture/sampler
handle IDs/generations, and scene shader/primitive handles before copying a
request into the pending queue. It also checks that each primitive supplies
every required shader vertex input by location/type; extra attributes are allowed.
Primitive index ranges must be nonempty, aligned to complete triangles, and
within the uploaded index count. An absent range selects the whole primitive.
A busy frame slot leaves the oldest request pending, so FIFO command processing alone
does not guarantee that a frame is recorded before a later destroy command.

Shader/texture/sampler/primitive destruction therefore calls `flush()`, then `waitIdle()`, then
releases the resource. Earlier accepted requests are recorded while their
resources and slots are still valid. Slot reuse happens only after those GPU
submissions finish. This stalls the render thread on release; the implementation
has no deferred GPU retirement queue or GPU reference-counting framework.

[`MetalResourcePool`](../src/engine/starlight/platform/metal/MetalResourcePool.cpp)
owns textures/samplers in separate 64-entry tables. Each free slot retains its
generation and increments it on destruction. A handle's ID is its logical shader
table index, independent of native argument-buffer slots. Recreated resources
can reuse indices, but old generations fail validation. Each draw's immutable
argument-table bytes prevent new resource creation from changing in-flight tables.

Shader destruction also removes and destroys cached graphics pipelines whose
key contains that shader, after pending frames finish and the device is idle.
It collects every matching cache key before destroying any pipeline. Allocation
failure during that collection propagates without destroying the shader or any
cached pipeline, so a caller that handles the exception can retry retirement.
The executor's [kcore thread wrapper](../external/kcore/src/runtime/Thread.cpp)
still treats uncaught worker exceptions as fatal.
Pipelines for other shaders remain cached. Later submissions with the destroyed
shader fail acceptance validation. Asset replacement creates a different
renderer handle; it does not retarget existing materials or request bytes.

Primitive destruction removes its record after releasing both buffers; later
submissions with a destroyed primitive fail validation. Repeated primitive
destruction is harmless. Direct device-buffer pool erasure remains immediate.

The [engine-thread asset layer](shader-pipeline.md#engine-thread-asset-ownership)
retains shader, texture, sampler, and geometry handles behind strong asset references and submits cleanup from
`update()`. Asset `shutdown()` waits for creation and release commands while the
renderer is still running; cleanup can submit additional release commands, which
would be rejected after executor stop closes the queue.

## Shutdown

[`RenderExecutor::stop()`](../src/engine/starlight/renderer/executor/RenderExecutor.cpp)
closes submission and joins the worker. On normal shutdown of a started executor,
the worker drains all commands accepted before closure, including creation,
release, and reply-bearing commands, in queue order. It continues ticking the
renderer between commands and flushes pending frames before exiting. Operation
failures still travel through their ordinary replies; later frame errors retain
their existing logging/drop behavior.

The worker observes closure before trying to pop: it exits only when that pop
finds an empty queue already known to be closed. This prevents a final accepted
push from being missed during shutdown. Calls after closure are rejected.
Stopping repeatedly is harmless. If the worker was never started, stop discards
queued commands without executing renderer operations; their abandoned promises
make async futures ready with `std::future_error` instead of leaving them pending.
The [proxy/executor tests](../src/engine/tests/asset/AssetSystem.test.cpp) cover
closure while commands are pending, reply results, and frame-before-release
ordering.

[`RenderingSystem::stop()`](../src/engine/starlight/renderer/RenderingSystem.cpp)
then calls `RenderDevice::waitIdle()`. Per-slot latches in
[`MetalDevice`](../src/engine/starlight/platform/metal/MetalDevice.cpp) signal GPU
frame completion independently of proxy flush completion.
