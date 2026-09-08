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
- Converts requests and scene data into a render graph.
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
    virtual void setPipeline(ComputePipelineHandle) = 0;
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

Resource creation uses immutable descriptors where practical. Graphics pipeline descriptions include shaders, vertex layouts, rasterization, depth/stencil, blending, attachment formats, and sample count.

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

1. Correct current frame-submission and lifetime behavior.
2. Add persistent `RenderTarget` attachments and resolve surfaces privately inside the backend recorder.
3. Render an empty scoped pass that clears and presents a surface.
4. Introduce the minimal frame-local render graph and record passes in insertion order.
5. Add texture resources and a texture-to-surface two-pass graph, then add stable topological ordering.
6. Add buffers, shaders, graphics pipelines, bind groups, and basic drawing.
7. Add uploads and copy passes.
8. Add semantic resource-state tracking and Vulkan synchronization2 translation.
9. Add depth/stencil, MSAA, and resolve attachments.
10. Add compute passes.
11. Add timestamps, debug labels, indirect commands, and deferred destruction.

## Deferred features

- Multiple GPU queues and queue ownership transfers.
- Async compute.
- Secondary Vulkan command buffers and parallel Metal encoders.
- Serializable command bytecode and capture/replay.
- Bindless resources and descriptor buffers.
- Metal argument-buffer optimization.
- Transient resource aliasing and pass merging.
- Mesh shaders and ray tracing.

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
