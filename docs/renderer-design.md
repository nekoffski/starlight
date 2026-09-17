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

The initial implementation uses one graphics-capable queue. Multiple queues, async compute, parallel recording, bindless resources, mesh shaders, and ray tracing are deferred until a concrete renderer feature needs them.

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

Vulkan maps bind groups to descriptor sets. Metal initially expands them into ordinary buffer, texture, and sampler binding calls. Metal argument buffers can later replace that implementation without changing the interface.

Shader compilation and cross-compilation belong to the asset pipeline, not command recording. The RHI receives backend-compatible compiled shader payloads and exposes them through `ShaderHandle`.

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
- Bindless resources and descriptor buffers.
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
