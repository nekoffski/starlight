# Renderer Design Plan

Status: working draft. This document is intentionally expected to change until the renderer and RHI shape is agreed.

## Goal

Build a renderer that:

- accepts backend-independent `RenderRequest`s;
- builds and compiles a render graph;
- records backend-independent graphics, compute, and copy commands;
- supports texture and surface outputs;
- supports multiple frames in flight;
- uses Metal and Vulkan adapters without exposing native objects to the renderer.

The initial implementation uses one graphics-capable queue. Multiple queues, async compute, parallel recording, mesh shaders, and ray tracing are deferred until a concrete renderer feature needs them. The preferred scalable shader-resource design uses hybrid binding; see [shader parameters and resource binding](shader-resource-research.md) for capability gates and implementation increments.

## Module ownership

```text
RendererProxy
    -> Renderer
        -> prepare scene items for each view/pass
            -> resolve persistent graphics pipelines
        -> frame-local RenderGraph
        -> ordered passes
            -> RenderDevice::trySubmitFrame()
                -> RenderFrameRecorder
                    -> render / compute / copy pass encoders
```

The modules own the following responsibilities.

### Renderer

- Accepts and queues `RenderRequest`s.
- Owns the logical frame number.
- Converts requests and scene data into prepared draws, then into a render graph.
- Derives portable pipeline descriptions from shader, geometry, material, and
  render-target compatibility state.
- Owns the logical graphics-pipeline cache and pipeline lifetime policy.
- Compiles graph dependencies when passes begin sharing resources.
- Chooses pass order.
- Issues RHI commands for compiled passes.
- Owns texture buffering policy when stable per-frame texture results are required.

### Render graph

- Represents passes as first-class nodes over persistent RHI handles.
- Records resource reads, writes, attachments, load/store operations, and dependencies.
- Detects invalid dependency cycles.
- Determines pass order and resource transitions.
- Can eventually determine transient resource lifetimes and aliasing.

### RHI and `RenderDevice`

- Creates and destroys GPU resources.
- Owns backend frame slots and GPU completion tracking.
- Provides a frame-scoped command recorder.
- Acquires surface images.
- Translates compiled resource usage into native synchronization.
- Submits recorded work and presents acquired surface images.
- Hides native Metal and Vulkan objects.

### Metal and Vulkan adapters

- Translate RHI descriptors and commands to native objects and commands.
- Own native resource pools, command buffers, encoders, queues, fences, semaphores, swapchains, and drawables.
- Handle resize, surface unavailability, device errors, and deferred destruction.

## Scene preparation and pipeline ownership

Application-facing code submits scene intent and views. It does not create or
own graphics pipelines. The temporary triangle-facing scene shape is allowed to
be small:

```cpp
struct RenderItem {
    u32 vertices;
    ShaderHandle shader;
};
```

This is scaffolding, not the final scene model. When meshes and materials exist,
the durable shape is closer to:

```cpp
struct RenderItem {
    MeshHandle mesh;
    MaterialHandle material;
    Mat4f transform;
};
```

Before building the frame graph, the renderer resolves the state needed to draw
each visible item in the pass that will consume it:

```text
scene item
    mesh       -> vertex layout and topology
    material   -> shader variant and fixed render state
    graph pass -> attachment formats and sample count
        -> GraphicsPipelineDescription
        -> resolve cached GraphicsPipelineHandle
        -> pipeline handle and draw arguments
```

For the first triangle, the graph callback can capture the resolved pipeline
handle and vertex count directly. Introduce a private, frame-local
`PreparedDraw` only when several draw sites need to carry the same pipeline,
buffer, binding, and draw-range data. Render-graph callbacks consume already
resolved state; they do not select or compile pipelines.

Pipeline creation is not exposed through `RendererProxy`. The renderer already
runs on the render thread and calls `RenderDevice` directly. Shader creation is
different: the asset system requests it through `RendererProxy` because shader
assets originate outside the renderer thread.

Preparation begins as private `Renderer` code, not as a new interface or class.
Extract a dedicated preparation or pipeline-cache module only when the renderer
contains enough policy to make that separation useful.

## Graphics pipeline model

A graphics pipeline is a persistent RHI resource. Reuse the existing
`GraphicsPipelineHandle`. Split graphics and compute handles only if supporting compute
pipelines makes the shared handle ambiguous in real code.

The renderer and RHI share one canonical, backend-independent description. The
initial triangle needs only the shader and its single color-target format:

```cpp
struct GraphicsPipelineDescription {
    TextureFormat format;
    ShaderHandle shader;

    bool operator==(const GraphicsPipelineDescription&) const = default;
};
```

The description itself is the logical cache identity. Do not maintain a second
partial `PipelineKey`, which could drift from creation state. It owns its values;
it must not contain non-owning spans or references when retained by the cache.

As corresponding renderer features are implemented, the description grows by
adding these groups:

```text
GraphicsPipelineDescription
    shader program / selected variant
    vertex-buffer layouts and attributes
    primitive and rasterization state
    multiple color-target formats, blending, and write masks
    optional depth/stencil format and state
    multisample state
    binding layout, only if shader reflection is no longer sufficient
```

Do not add those types before their feature exists. Keeping the creation
interface descriptor-based allows fields to be added without growing an
argument list:

```cpp
class RenderDevice {
   public:
    virtual Result<GraphicsPipelineHandle> createGraphicsPipeline(
        const GraphicsPipelineDescription& description
    ) = 0;

    virtual void destroyGraphicsPipeline(
        GraphicsPipelineHandle pipeline
    ) = 0;
};
```

The renderer initially resolves pipelines with a private function and the
existing linear `FlatMap`:

```cpp
Result<GraphicsPipelineHandle> resolveGraphicsPipeline(
    const GraphicsPipelineDescription& description
);

FlatMap<GraphicsPipelineDescription, GraphicsPipelineHandle> m_graphicsPipelines;
```

Keep created pipelines until renderer shutdown for the initial implementation.
This avoids eviction and destruction of resources referenced by in-flight work.
Add dependency tracking, hot-reload invalidation, deferred destruction, a hash
map, disk caches, and asynchronous compilation only when required.

The description contains compatibility state, not scene or frame identity:

- no `MeshHandle`, `MaterialHandle`, texture handle, or surface handle;
- no distinction between surface and texture outputs;
- no frame, view, or pass identifier;
- no bound resources or transforms;
- no viewport, scissor, blend constant, stencil reference, or depth bias when
  those remain dynamic encoder commands.

Surface and texture targets with the same attachment formats and sample count
share a pipeline. The initial descriptor has one color format and implicitly
uses one sample. The renderer therefore needs portable format metadata for its
targets; it must not inspect native drawables or Vulkan images to derive it.
For now the renderer uses the single supported `TextureFormat::bgra8unorm`,
which must match the format configured on the Metal surface. When formats
become configurable, the renderer retains the format selected during target
creation; it does not query it back from the backend.

On Metal, one logical `MetalGraphicsPipeline` may own an
`MTLRenderPipelineState`, an `MTLDepthStencilState`, and portable values applied
dynamically by the encoder. On Vulkan it may map primarily to `VkPipeline` plus
its compatible layout. One RHI handle represents the complete logical state
bundle, not necessarily exactly one native object.

## Render-pass decision

A render pass is a first-class concept in the renderer and render graph, but not a persistent backend resource.

The renderer-side graph pass contains semantic intent:

- pass name;
- color and depth/stencil attachments;
- load, clear, and store operations;
- resource reads and writes;
- the commands used to execute the pass.

After graph compilation, the renderer sends a transient `RenderPassDesc` to the RHI. The backend translates it immediately:

- Metal creates an `MTLRenderPassDescriptor` and `MTLRenderCommandEncoder`.
- Vulkan uses dynamic rendering through `vkCmdBeginRendering` and `vkCmdEndRendering`.

The backend must not own renderer-level pass nodes or graph dependencies. Conversely, the renderer must not know about `MTLRenderPassDescriptor`, `VkRenderingInfo`, image layouts, pipeline stages, or native synchronization primitives.

This gives the renderer enough information to build a graph while allowing each backend to choose its native implementation.

## Render graph shape

The initial graph uses existing `TextureHandle` and `SurfaceHandle` values directly. It does not add graph-local image wrappers or a public builder/compiler hierarchy.

```cpp
struct GraphRenderPass {
    Str label;
    std::vector<ColorAttachment> colors;
    std::vector<TextureHandle> reads;
    RenderFrameRecorder::RenderPassCallback record;
};

class RenderGraph {
   public:
    void addRenderPass(GraphRenderPass pass);
    Result<void> record(RenderFrameRecorder& recorder);

   private:
    std::vector<GraphRenderPass> m_passes;
};

RenderGraph graph;

graph.addRenderPass({
    .label = "scene",
    .colors = {{
        .target = sceneTexture,
        .loadOp = LoadOp::clear,
        .storeOp = StoreOp::store,
        .clearColor = clearColor,
    }},
    .record = recordScene,
});

graph.addRenderPass({
    .label = "present",
    .colors = {{
        .target = surface,
        .loadOp = LoadOp::discard,
        .storeOp = StoreOp::store,
    }},
    .reads = {sceneTexture},
    .record = [sceneTexture](RenderPassEncoder& encoder) -> Result<void> {
        // Bind sceneTexture and draw a fullscreen triangle.
        return {};
    },
});
```

Color attachments declare writes and `reads` declares sampled texture dependencies. The execution callback records how the pass performs its work. The first implementation records passes in insertion order. Stable topological ordering is added with the first real multi-pass dependency.

The renderer owns intermediate textures, including any ring required for stable frames-in-flight results, so callbacks can capture concrete `TextureHandle`s. A graph-local image handle and resource resolver are added only when the graph itself creates transient textures. Transient aliasing and pass merging remain deferred.

## Frame submission interface

```cpp
using RecordFrame =
    MoveOnlyFunction<Result<void>(RenderFrameRecorder&)>;

class RenderDevice {
   public:
    virtual ~RenderDevice() = default;

    virtual Result<void> trySubmitFrame(RecordFrame record) = 0;
    virtual void waitIdle() = 0;

    // Resource creation and destruction methods.
};
```

If no frame slot is available, submission returns `ErrorCode::tooManyFramesInFlight`. The renderer keeps the pending request and retries it later. A successful submission returns after native queue submission, not after GPU completion.

The recording callback returns `Result<void>` so resource lookup, surface acquisition, encoder creation, and validation failures can abort recording safely.

## Frame recorder

```cpp
class RenderFrameRecorder {
   public:
    using RenderPassCallback =
        MoveOnlyFunction<Result<void>(RenderPassEncoder&)>;

    virtual ~RenderFrameRecorder() = default;

    virtual Result<void> renderPass(
        RenderPassCallback callback,
        const RenderPassDescription& description
    ) = 0;
};
```

Pass callbacks enforce valid encoder lifetimes. Only one pass encoder may be active at a time. Compute and copy pass methods are added when those pass types are implemented.

Surface acquisition is not exposed through this interface. A `SurfaceHandle` is supplied as a render-pass attachment. The backend resolves it immediately before creating the native pass encoder, acquires or reuses one frame-local drawable, and remembers it for presentation. `ErrorCode::renderSurfaceNotDrawable` skips only the affected surface pass; other errors abort frame recording.

## Attachments and clearing

Clearing is an attachment load operation, not a `clearSurface()` or `clearTexture()` command.

```cpp
using RenderTarget = std::variant<SurfaceHandle, TextureHandle>;

enum class LoadOp {
    load,
    clear,
    discard,
};

enum class StoreOp {
    store,
    discard,
};

struct ColorAttachment {
    RenderTarget target;
    LoadOp loadOp{LoadOp::load};
    StoreOp storeOp{StoreOp::store};
    Vec4f clearColor{};
};

struct DepthStencilAttachment {
    TextureHandle target;
    LoadOp depthLoadOp{LoadOp::load};
    StoreOp depthStoreOp{StoreOp::store};
    f32 clearDepth{1.0f};
    LoadOp stencilLoadOp{LoadOp::load};
    StoreOp stencilStoreOp{StoreOp::store};
    u32 clearStencil{0};
};

struct RenderPassDescription {
    Str label;
    std::span<const ColorAttachment> colorAttachments;
    Opt<DepthStencilAttachment> depthStencil;
    std::span<const ResourceUse> resources;
};
```

`TextureHandle` initially means the full texture. Introduce texture-view handles only when rendering to individual mip levels, array layers, or reinterpreted formats is implemented.

An empty render-pass callback with a color attachment using `LoadOp::clear` is the first end-to-end rendering milestone.

## Pass encoder interfaces

### Graphics

```cpp
class RenderPassEncoder {
   public:
    virtual void setPipeline(GraphicsPipelineHandle) = 0;
    virtual void setBindGroup(
        u32 index,
        BindGroupHandle,
        std::span<const u32> dynamicOffsets = {}
    ) = 0;
    virtual void setVertexBuffer(u32 slot, BufferHandle, u64 offset = 0) = 0;
    virtual void setIndexBuffer(
        BufferHandle,
        IndexType,
        u64 offset = 0
    ) = 0;

    virtual void setViewport(const Viewport&) = 0;
    virtual void setScissor(const RectU&) = 0;
    virtual void setBlendConstant(const Vec4f&) = 0;
    virtual void setStencilReference(u32) = 0;
    virtual void setDepthBias(f32 constant, f32 slope, f32 clamp) = 0;

    virtual void pushConstants(
        ShaderStages,
        u32 offset,
        CBytesView
    ) = 0;

    virtual void draw(
        u32 vertexCount,
        u32 instanceCount = 1,
        u32 firstVertex = 0,
        u32 firstInstance = 0
    ) = 0;

    virtual void drawIndexed(
        u32 indexCount,
        u32 instanceCount = 1,
        u32 firstIndex = 0,
        i32 baseVertex = 0,
        u32 firstInstance = 0
    ) = 0;

    virtual void drawIndirect(BufferHandle, u64 offset) = 0;
    virtual void drawIndexedIndirect(BufferHandle, u64 offset) = 0;
};
```

### Compute

```cpp
class ComputePassEncoder {
   public:
    virtual void setPipeline(ComputeGraphicsPipelineHandle) = 0;
    virtual void setBindGroup(
        u32 index,
        BindGroupHandle,
        std::span<const u32> dynamicOffsets = {}
    ) = 0;
    virtual void pushConstants(u32 offset, CBytesView) = 0;
    virtual void dispatch(u32 x, u32 y, u32 z) = 0;
    virtual void dispatchIndirect(BufferHandle, u64 offset) = 0;
};
```

### Copy

```cpp
class CopyPassEncoder {
   public:
    virtual void copyBuffer(const BufferCopy&) = 0;
    virtual void copyBufferToTexture(const BufferTextureCopy&) = 0;
    virtual void copyTextureToBuffer(const TextureBufferCopy&) = 0;
    virtual void copyTexture(const TextureCopy&) = 0;
    virtual void fillBuffer(
        BufferHandle,
        u64 offset,
        u64 size,
        u32 value
    ) = 0;
    virtual void generateMipmaps(TextureHandle) = 0;
};
```

## Resources and bindings

The RHI requires generational handles for:

- buffers;
- textures;
- texture views;
- samplers;
- shaders;
- bind-group layouts;
- bind groups;
- graphics pipelines;
- compute pipelines;
- timestamp pools.

Resource creation uses immutable descriptors where practical. The first
graphics-pipeline description contains only its shader and one color format. It
grows to include vertex layouts, rasterization, depth/stencil, blending,
multiple attachment formats, and sample count only as those features are added.

Bindings use immutable bind groups:

```cpp
enum class BindingType {
    uniformBuffer,
    readOnlyStorageBuffer,
    storageBuffer,
    sampledTexture,
    storageTexture,
    sampler,
};
```

Logical bind groups lower through the target-specific reflected shader ABI.
Vulkan initially uses descriptor sets; Metal uses direct bindings or argument
buffers to match the compiled declarations. A Slang `ParameterBlock` containing
resources can require an argument buffer, so expanding it into individual
resource calls requires a corresponding shader variant. Hybrid binding,
uniform packing, indexed resource tables, and an optional Vulkan descriptor-heap
path are detailed in [shader parameters and resource binding](shader-resource-research.md).

Shader compilation and cross-compilation belong to the asset pipeline, not command recording. The RHI receives backend-compatible compiled shader payloads and exposes them through `ShaderHandle`.

### Bind groups, update frequency, and bindless selection

This section records the proposed shader-parameter contract; these interfaces
are not implemented yet. A bind-group layout describes expected bindings and
their types. A bind group supplies actual buffer slices, textures, samplers,
or resource arrays for that layout. `frameGroup`, `sceneGroup`, and
`resourceGroup` are all instances represented by `BindGroupHandle`, with
different layouts and contents. Their names describe their purpose, not
different RHI types. Recording validates compatibility with the pipeline's
layout. The group index in `setBindGroup` is a frontend pipeline-layout index;
backend lowering resolves its native binding representation.

Keep three decisions separate:

| Decision | Owner | Example |
| --- | --- | --- |
| Field types, offsets, and bindings | Shader declarations and target reflection | A model matrix is a `float4x4` |
| Storage and upload strategy | Renderer policy | Model records live in a structured buffer |
| When values change | Application changes tracked by the renderer | Moving an object updates its transform |

Reflection does not infer update frequency. Setting a value, uploading it,
and binding the prepared resources are separate operations. Immutable bind
groups do not imply that their referenced buffers never change; buffer writes
must obey submission lifetime and synchronization rules.

The conventional path retains the familiar frame/pass, material, and object
frequencies. Bind frame/pass parameters once, bind a material when it changes,
and select an object's constant-buffer slice for each draw. Vulkan can use
dynamic offsets where the compiled layout supports them. Frame and pass can
share a group or remain separate when their reuse differs.

The preferred scalable path changes selection, while preserving those logical
update frequencies:

| Logical role | Bound contents | Shader selection |
| --- | --- | --- |
| Frame/view and pass | Buffer-backed constants and explicit pass resources | Named fields/resources |
| Scene | Object-record and material-record buffer slices | Object/material indices |
| Resources | Typed texture and sampler arrays | Indices stored in material records |
| Draw | Small object/material indices and flags | Immediate draw parameters |

The scene group has a binding for each record buffer, not a binding for every
object or material. The resource group binds tables rather than a particular
material's textures. Scene buffers and resource tables may share a group if
the program layout supports it; these roles do not mandate a fixed number of
native descriptor sets.

```text
draw.objectIndex   -> objects[index]   -> model matrix
draw.materialIndex -> materials[index] -> color, flags, texture/sampler indices
                                      -> resources.textures[textureIndex]
                                      -> resources.samplers[samplerIndex]
```

Bindless still binds the resource tables. It moves individual resource
selection from CPU binding commands to shader indexing. Indexed object and
material records are ordinary buffer access; texture/sampler table selection
is the bindless part. A bounded table qualifies. Ordinary frame/pass bindings
plus bindless material resources form the hybrid design.

### Parameter authoring and draw data

The user-facing parameter/material interface accepts typed matrices, colors,
flags, and engine resource handles. It resolves names once into layout-qualified
field IDs, packs values using target reflection, and prepares frame-safe
bindings. The RHI bind group supplies those prepared resources; it does not
interpret camera or material semantics. Users assign a texture handle to a
material, and the renderer owns table-slot allocation and writes the resulting
index into the GPU record. Native descriptor bytes and table indices are not
application-facing resource handles.

General reflected parameter groups support user-defined shaders. Indexed
materials initially opt into a specified material-record schema; arbitrary
reflected blocks do not automatically fit that schema. Conventional and
bindless shader variants can expose the same logical material parameters,
but require their own reflected native layouts.

`DrawData` is a small logical block containing object/material indices and
flags. A proposed `setDrawData` encoder operation lowers to Vulkan push
constants on the descriptor-set path, or Metal copied bytes/buffer slices.
Its layout, size, and stage visibility belong to the shader artifact and
pipeline layout. The optional Vulkan descriptor-heap path uses its push-data
mechanism instead. Large transforms and material records remain in buffers.

### Storage and update lifetime

Start with CPU-owned frame, object, and material values plus GPU versions
associated with reusable frame slots. A slot becomes writable only after its
previous GPU work completes. Frame uploads and recorded parameter snapshots
remain valid through completion; persistent texture allocations need not be
duplicated with each frame's table version.

Frame/view values are prepared for each view; object records change when
transforms or object values change; material records change through material
setters. Resource-table contents change when resources are registered,
replaced, or removed. Draw selectors are recorded per draw. Thus a group can
stay bound while its records are selected at per-object frequency.

Initially upload all active records into each reusable frame slot. If selective
uploads become useful, track the CPU record version and uploaded version for
each frame slot. A single dirty flag cleared after updating one slot leaves
other slots stale. Bindless slots additionally require deferred recycling
until live CPU references and pending GPU submissions release their old use;
material indices and the bound table version must agree.

## Resource synchronization

The RHI does not expose Vulkan layouts, access flags, or pipeline stages directly.

```cpp
enum class ResourceAccess {
    vertexRead,
    indexRead,
    indirectRead,
    uniformRead,
    sampledRead,
    storageRead,
    storageWrite,
    storageReadWrite,
    copyRead,
    copyWrite,
};

struct ResourceUse {
    std::variant<BufferRange, TextureViewHandle> resource;
    ResourceAccess access;
    ShaderStages stages;
};
```

Attachments infer color/depth usage. Copy operations infer source/destination usage. Other resources are declared on the graph pass and carried into the compiled `RenderPassDesc` or `ComputePassDesc`.

The Vulkan adapter translates state changes into synchronization2 barriers and image layout transitions. The Metal adapter initially relies on default tracked hazard mode. Resource state persists between passes and frame submissions.

Dependencies within one pass that require a barrier should initially be represented as two passes. An explicit intra-pass barrier command can be added later if profiling demonstrates a need.

## Render-target behavior

### Surface output

- A `SurfaceHandle` is used directly as a render attachment.
- The backend recorder lazily acquires or reuses one native surface image when the pass begins.
- Successful frame submission schedules presentation.
- Frame-slot completion and presentation-image availability are tracked separately.

### Texture output

- A `TextureHandle` is used directly as a render attachment.
- It is never acquired or presented.
- Repeated writes to one texture produce latest-result semantics.
- If each in-flight frame needs a stable result, the renderer owns a ring of textures and selects one using its frame index.

### Texture followed by surface

The graph represents this as two passes:

```text
scene pass:   writes scene texture target
present pass: reads scene texture, writes surface target
```

The graph dependency causes the backend to synchronize the write-to-read transition.

If `RenderFrameRecorder::renderPass()` returns `ErrorCode::renderSurfaceNotDrawable`, texture passes may still execute and the surface pass is skipped.

## Backend frame contexts

### Metal

```cpp
struct MetalRenderFrameLatch {
    std::atomic_bool available{true};

    // Add when required:
    // UploadArena uploadArena;
    // TransientBindingArena bindings;
    // DeferredReleaseQueue garbage;
};
```

Metal creates one fresh `MTLCommandBuffer` per submitted frame. Each RHI pass creates and finishes one corresponding Metal encoder. The command buffer completion handler releases the slot. Command buffers, encoders, and drawables are not persistent members of the frame context.

### Vulkan

```cpp
struct VulkanRenderFrameContext {
    VkFence completionFence;
    VkCommandPool commandPool;
    VkCommandBuffer commandBuffer;

    // Add when required:
    // TransientDescriptorPool descriptors;
    // UploadArena uploadArena;
    // DeferredReleaseQueue garbage;
};
```

The Vulkan adapter resets the command pool only after the slot fence signals. Presentation-finished semaphores belong to swapchain images rather than frame slots.

## Metal adapter shape

```text
MetalDevice
    MetalContext
    MetalResourcePool
    vector<MetalRenderFrameLatch>

MetalRenderFrameRecorder
    MTLCommandBuffer*
    MetalResourcePool&
    acquired drawables
    resource-state tracker

MetalRenderPassEncoder
    MTLRenderCommandEncoder*

MetalComputePassEncoder
    MTLComputeCommandEncoder*

MetalCopyPassEncoder
    MTLBlitCommandEncoder*
```

`MetalRenderFrameRecorder::renderPass()` resolves texture targets directly and surface targets through a private drawable-acquisition helper. It reuses a drawable when multiple passes target the same surface, creates an `MTLRenderCommandEncoder`, invokes the graphics callback, and always ends encoding. After all frame recording succeeds, `MetalDevice` schedules every acquired drawable for presentation and commits the command buffer.

## Vulkan adapter shape

```text
VulkanDevice
    VulkanResourcePool
    vector<VulkanRenderFrameContext>

VulkanRenderFrameRecorder
    VkCommandBuffer
    VulkanResourcePool&
    acquired swapchain images
    resource-state tracker
```

The initial Vulkan baseline is Vulkan 1.3 dynamic rendering, synchronization2, `vkQueueSubmit2`, and one primary command buffer per frame slot.

## Remaining correctness work

Before extending command recording:

1. Validate `maxFramesInFlight > 0` and use `size_t` for slot indexing.
2. Implement actual surface destruction and deferred native-resource release.
3. Configure surface pixel format and drawable size, then update drawable size on resize.

## Implementation plan

1. Close the remaining frame, surface-format, resize, and resource-lifetime
   correctness gaps listed above. Explicitly configure Metal surfaces as
   `bgra8unorm` so they match the initial pipeline description.
2. Reuse the existing `GraphicsPipelineHandle`; add the current minimal
   `GraphicsPipelineDescription` and RHI create/destroy methods.
3. Implement `MetalGraphicsPipeline` and store it in `MetalResourcePool`.
   Validate the shader stages and set the Metal color format from the portable
   description.
4. Add only `RenderPassEncoder::setPipeline()` and `draw()`; leave the other
   encoder commands until a feature uses them.
5. In private `Renderer` code, resolve the description through the existing
   `FlatMap` before adding the graph pass. Capture the pipeline handle and
   vertex count in the callback and render the triangle.
6. Add vertex and index buffers, then replace the temporary vertex-count scene
   item with mesh/material-driven preparation.
7. Add texture resources and a texture-to-surface two-pass graph, followed by
   stable topological ordering and portable resource transitions.
8. Add reflected resource bindings and uploads when the first shader consumes
   external data.
9. Add depth/stencil, blending, MSAA, and multiple render targets one feature at
   a time, extending the pipeline description with the required state.
10. Add the Vulkan implementation against the same RHI descriptions and
    encoder contract.
11. Add compute, timestamps, indirect commands, and advanced lifetime/cache
    policies only when renderer features require them.

## Deferred features

- Multiple GPU queues and queue ownership transfers.
- Async compute.
- Secondary Vulkan command buffers and parallel Metal encoders.
- Serializable command bytecode and capture/replay.
- Unbounded universal bindless heaps; the bounded hybrid resource-table path is planned in [shader-resource-research.md](shader-resource-research.md).
- Metal argument-buffer optimization.
- Transient resource aliasing and pass merging.
- Mesh shaders and ray tracing.

## Architecture validation

The selected boundary matches established renderer designs without requiring
their full machinery:

- Filament accepts a scene through a `View` and keeps command and pipeline work
  behind `Renderer`. This supports keeping pipeline creation out of the public
  scene API.
- Bevy prepares render items, specializes a pipeline using target format and
  mesh state, and records the cached pipeline ID later in a render pass. This
  supports resolving a pipeline before graph-pass recording.
- wgpu creates an immutable render pipeline from one descriptor and describes
  render passes separately. This supports the descriptor-based RHI seam and
  the separation between persistent pipelines and transient passes.
- bgfx exposes program/state submission directly because it is a low-level
  rendering library. Starlight is choosing the higher-level scene-renderer
  boundary instead, so copying that public API would leak RHI policy upward.

The architecture is not overengineered if preparation stays as private
`Renderer` code and the cache begins as a linear collection. Do not add a
public pipeline API, `PipelineRecipe`, `PreparedFrame`, standalone cache class,
hashing, eviction, asynchronous compilation, or a complete speculative state
model now. The stable seam is the descriptor passed from `Renderer` to
`RenderDevice`; its fields are expected to grow with implemented features.

## Buffer resource contract research

The asset system requests preparation of logical render resources. It does not
allocate GPU memory, select heaps or memory types, manage staging buffers, or
choose dedicated versus suballocated storage.

The stable ownership flow is:

```text
asset loader
    -> CPU mesh data
    -> RendererProxy::uploadMeshAsync(move(upload))
    -> renderer-owned MeshHandle
    -> internal DeviceBufferHandle values
    -> backend-private allocation and upload
```

The public asset-to-renderer operation should be atomic at mesh granularity.
One failed vertex or index upload fails the mesh and rolls back its already
created buffers. `DeviceBufferHandle` remains an internal renderer/RHI concept;
the asset system should not coordinate several buffer futures or expose partial
mesh readiness.

At the RHI seam, a buffer handle denotes a logical byte resource, not
necessarily one native allocation. Today it may map to a dedicated
`MTL::Buffer`. Later it may resolve to a backing buffer, base offset, and size
inside a Metal heap or Vulkan allocator without changing mesh or encoder
interfaces. Encoder offsets are relative to that logical resource.

A successful upload means the returned mesh handle is safe to reference from
later renderer submissions. It does not require the GPU copy to have completed.
The backend must copy caller-owned bytes into backend-owned memory before the
request returns and order any pending transfer before the first consuming draw.
Native destruction is likewise deferred until earlier GPU submissions no
longer reference the allocation.

This shape is supported by existing systems:

- Metal provides both direct buffer initialization by copying caller bytes and
  heap-backed buffer creation, so physical placement is an implementation
  choice rather than asset metadata.
- Vulkan separates buffers, memory allocation, staging copies, and transfer
  synchronization; exposing those choices to asset loaders would leak backend
  policy.
- wgpu exposes logical buffers and queue writes. `write_buffer` copies caller
  data into staging immediately while GPU execution begins with a later queue
  submission.
- bgfx returns logical resource handles immediately and defers the actual
  creation/upload commands to its render thread before dependent draws.
- Filament creates logical vertex/index resources separately from supplying
  their buffer data, and its asset loader transfers owned data through buffer
  descriptors and completion callbacks.
- Bevy keeps CPU source assets separate from prepared render assets. Its fully
  demand-driven extraction/preparation system is a possible later evolution,
  but would add asset IDs, versioning, retry scheduling, and cache eviction that
  Starlight does not yet need.

Therefore the initial implementation may keep
`RenderDevice::createBuffer(description-with-owned-bytes)` as a convenience for
immutable static data, provided it preserves the readiness and deferred-release
contract above. If dynamic updates or upload batching become real requirements,
split internal allocation from `writeBuffer()` without changing the external
mesh-upload interface.

## Shader reflection and vertex data design

Decision recorded: 2026-09-29. This is the agreed implementation direction;
the types below are a design sketch, not implemented declarations.

The shader describes its required inputs, the primitive describes its stored
bytes, and pipeline creation matches the two. Slang reflection supplies the
shader interface; it cannot infer the byte offsets, strides, storage formats,
or streams of user-provided geometry.

### Portable interface and ownership

Extend the existing shader, primitive, pipeline, and encoder descriptions:

```cpp
struct VertexAttribute {
    Semantic semantic;       // name and index, e.g. POSITION0 or TEXCOORD1
    VertexFormat format;     // storage format, e.g. float32x3
    u32 stream;
    u32 offset;
};

struct VertexStreamLayout {
    u32 stride;
    InputRate rate;          // per vertex initially; per instance when needed
};

struct VertexLayout {
    std::vector<VertexStreamLayout> streams;
    std::vector<VertexAttribute> attributes;
};

struct ShaderVertexInput {
    Semantic semantic;
    ShaderValueType type;    // type expected by the shader
    u32 location;            // reflected from this target's compiled artifact
};
```

- `PrimitiveUploadData` supplies owned bytes for each stream and a
  `VertexLayout`. Owned bytes survive submission to the renderer thread. A
  small typed helper can describe C++ structs with `sizeof` and `offsetof`.
- Uploaded primitives retain their layout alongside buffer handles and draw
  ranges. Buffer handles and buffer-slice offsets are not layout identity.
- `ShaderLoader` parses the selected vertex entry point's
  `scope.parameters` into `ShaderModuleDescription`. Prefer the scope
  representation in Slang JSON 1.1; do not also traverse its legacy duplicate
  `parameters` array. Flatten supported structs and reject unsupported input
  shapes explicitly.
- Data-driven vertex inputs declare explicit semantics such as `POSITION0`.
  Match the semantic name and index; an omitted JSON `semanticIndex` means
  zero. System inputs such as `SV_VertexID` require no vertex attribute.
- Reflection belongs to the compiled target artifact. Keep Metal and SPIR-V
  reflection paired with their respective binaries, generated in the same
  build step. Numeric locations are not persistent semantic identities.

### Matching, validation, and pipeline identity

Use one shared pure function to match reflected shader requirements against
the primitive layout. Its result describes each consumed attribute using
shader location, storage format, logical stream, and byte offset, together
with the required stream strides and input rates.

Extra primitive attributes are allowed. Missing or incompatible required
attributes produce an error naming the semantic and expected type. Validate
duplicate semantics and locations, stream references, byte extents, and
upload sizes. Storage format and shader type remain separate: normalized
integer storage can feed floating-point shader inputs when supported. Start
with the formats actually used and add explicit compatibility rules as needed.

`GraphicsPipelineDescription` owns a `VertexLayout` value alongside shader
and target compatibility state. Its existing equality-based cache identity
therefore distinguishes physical layouts while allowing different meshes
with the same layout to share pipelines. Do not retain spans into upload data
or put buffer handles in the pipeline key.

Keep compilation and matching out of draw-command recording where possible:
resolve pipeline state before recording the graph callback. No public pipeline
creation interface or separate shader-system class is needed.

### Metal and Vulkan adapters

| Portable description | Metal | Vulkan |
| --- | --- | --- |
| Shader input location | `MTLVertexDescriptor.attributes[location]` | `VkVertexInputAttributeDescription.location` |
| Storage format and offset | Vertex attribute descriptor | Vertex attribute description |
| Stream stride and input rate | Vertex buffer layout descriptor | Vertex input binding description |
| Logical stream binding | Map to a native vertex-stage buffer slot | Vertex buffer binding number |

Change `RenderPassEncoder::setVertexBuffer` to accept a logical stream number
and `DeviceBufferSlice`. The Metal pipeline retains a logical-to-native slot
mapping; the encoder applies it when binding buffers. Vulkan can normally use
the logical stream number directly.

Metal vertex data and other vertex-stage buffer arguments share the buffer
argument table. The Metal adapter must choose geometry slots that do not
overlap reflected constant/storage/argument-buffer bindings. Reflect occupied
resource slots, allocate free geometry slots at pipeline creation, and reject
layouts that exceed backend limits. Keep this native allocation private to
the adapter; user geometry must not encode Metal buffer indices.

### Validation evidence and design corrections

During the 2026-09-25 investigation, local Slang probes with `POSITION0`,
`NORMAL0`, and `TEXCOORD0` compiled to Metal and SPIR-V. Both reflection outputs
reported input locations 0, 1, and 2; the generated Metal attributes matched.
A second probe confirmed that `TEXCOORD1` carries `semanticIndex: 1`, while
index zero was omitted.

A probe containing vertex inputs and a constant buffer emitted Metal
`[[attribute(0)]]` and `[[buffer(0)]]` in the same vertex function. Since the
descriptor's `bufferIndex` references the same buffer table, assigning geometry
to native slot zero would overlap that resource. Its reflection also differed
by target: Metal reported `constantBuffer`, while SPIR-V reported
`descriptorTableSlot`. These were compiler/reflection checks, not Vulkan
rendering tests; Vulkan runtime behavior remains to be verified.

The initial proposal was incomplete about numeric-location identity and
Metal resource-slot overlap. Semantic matching plus private native slot
allocation addresses both. A shader-only layout generator would still need
an imposed packing convention or mesh repacking, so it does not satisfy the
goal of flexible user-provided bytes.

### Implementation order

1. Add portable vertex layouts and owned stream uploads, supporting the
   formats currently used.
2. Retain and validate vertex input reflection from the existing Slang JSON;
   make binary and metadata generation one shader build step.
3. Add the shared matcher, extend the pipeline key, and implement Metal
   descriptor translation and stream binding. Verify an input-free
   `SV_VertexID` shader, position-only input, multiple attributes, two physical
   layouts for the same shader, and a missing required attribute.
4. Implement the equivalent Vulkan vertex input translation and verify the
   same cases with Vulkan validation enabled.
5. Extend resource reflection and material data when the first uniforms or
   textures are consumed. Preserve the same pattern: reflected requirements,
   user-provided values, and target-specific binding inside the adapter.

Use the existing JSON dependency and offline Slang compiler. A runtime Slang
compiler dependency, generic material property system, and broad format table
are deferred until concrete features need them.

This follows the separation demonstrated by
[wgpu vertex buffer layouts](https://wgpu.rs/doc/wgpu/struct.VertexBufferLayout.html)
and [Bevy mesh pipeline specialization](https://bevy.org/examples-webgpu/shaders/specialized-mesh-pipeline/):
geometry supplies its physical layout, and pipeline preparation selects the
attributes required by the shader.

## References

- [Apple: setting up a command structure](https://developer.apple.com/documentation/Metal/setting-up-a-command-structure)
- [Apple: Metal command buffers](https://developer.apple.com/documentation/metal/mtlcommandbuffer)
- [Apple: Metal command encoders](https://developer.apple.com/documentation/metal/mtlcommandencoder)
- [Apple: load and store actions](https://developer.apple.com/documentation/Metal/setting-load-and-store-actions)
- [Apple: resource synchronization](https://developer.apple.com/documentation/metal/resource-synchronization)
- [Apple: argument buffers](https://developer.apple.com/documentation/metal/managing-groups-of-resources-with-argument-buffers)
- [Khronos: Vulkan command buffers](https://docs.vulkan.org/spec/latest/chapters/cmdbuffers.html)
- [Khronos: dynamic rendering](https://docs.vulkan.org/samples/latest/samples/extensions/dynamic_rendering/README.html)
- [Khronos: synchronization2](https://docs.vulkan.org/guide/latest/extensions/VK_KHR_synchronization2.html)
- [Khronos: descriptor sets](https://docs.vulkan.org/spec/latest/chapters/descriptorsets.html)
- [Khronos: swapchain semaphore reuse](https://docs.vulkan.org/guide/latest/swapchain_semaphore_reuse.html)
- [Filament: `Renderer` and `View` boundary](https://github.com/google/filament/blob/main/filament/include/filament/Renderer.h)
- [Bevy: manual mesh pipeline preparation and specialization](https://github.com/bevyengine/bevy/blob/main/examples/2d/mesh2d_manual.rs)
- [wgpu: render-pipeline descriptor implementation](https://github.com/gfx-rs/wgpu/blob/trunk/wgpu-core/src/pipeline.rs)
- [bgfx: explicit low-level program/state submission](https://github.com/bkaradzic/bgfx/blob/master/examples/06-bump/bump.cpp)
- [Apple: `MTLBuffer` creation and storage](https://developer.apple.com/documentation/metal/mtlbuffer)
- [Apple: heap-backed buffer creation](https://developer.apple.com/documentation/metal/mtlheap/makebuffer%28length%3Aoptions%3A%29)
- [Khronos: staging vertex data into device-local memory](https://docs.vulkan.org/tutorial/latest/04_Vertex_buffers/02_Staging_buffer.html)
- [Khronos: buffer-upload synchronization examples](https://docs.vulkan.org/guide/latest/synchronization_examples.html)
- [wgpu: buffer initialization and upload paths](https://docs.rs/wgpu/latest/wgpu/struct.Buffer.html)
- [wgpu: queue-write ordering and staging semantics](https://docs.rs/wgpu/latest/wgpu/struct.Queue.html#method.write_buffer)
- [bgfx: deferred resource API](https://bkaradzic.github.io/bgfx/internals.html#resource-api)
- [Filament: vertex-buffer data upload](https://github.com/google/filament/blob/main/filament/src/VertexBuffer.cpp)
- [Filament: glTF resource upload path](https://github.com/google/filament/blob/main/libs/gltfio/src/ResourceLoader.cpp)
- [Bevy: CPU-to-render asset preparation](https://docs.rs/bevy/latest/bevy/render/render_asset/trait.RenderAsset.html)
- [Slang: reflection API and JSON scopes](https://docs.shader-slang.org/en/stable/external/slang/docs/user-guide/09-reflection.html)
- [Slang: Metal entry-point transformations](https://docs.shader-slang.org/en/stable/external/slang/docs/user-guide/a2-02-metal-target-specific.html)
- [Apple: vertex attribute descriptors](https://developer.apple.com/documentation/metal/mtlvertexattributedescriptor)
- [Apple: vertex attribute buffer indices](https://developer.apple.com/documentation/metal/mtlvertexattributedescriptor/bufferindex)
- [Khronos: fixed-function vertex input](https://docs.vulkan.org/spec/latest/chapters/fxvertex.html)
