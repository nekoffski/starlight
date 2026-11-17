# Geometry assets

[`AssetSystem::createGeometry()`](../src/engine/starlight/asset/AssetSystem.cpp)
accepts owned CPU [`GeometryData`](../src/engine/starlight/geometry/GeometryData.hh)
and returns a strong
[`GeometryRef`](../src/engine/starlight/asset/Geometry.hh). Creation uses the
existing asynchronous primitive command; the command owns copies of the vertex
and index bytes. `update()` publishes the resulting `PrimitiveHandle`, or exposes
the creation error through the reference. `waitForPending()` provides blocking
startup publication. Geometry assets are immutable after creation.

Copies of a reference retain the same geometry. Independent creation calls make
distinct assets. References expose the shared asset readiness/error/version
interface; they stay on the asset system's engine thread. Retain a reference
through draw preparation and submission, then release it before asset shutdown.
The [sandbox](../src/sandbox/main.cpp) creates a quad through this path and shares
one primitive across both instances. Each instance draws its two triangle
sections with separate nearest/linear checkerboard materials.

## CPU geometry data

[`geometry/GeometryData.hh`](../src/engine/starlight/geometry/GeometryData.hh)
contains the vertex layout, owned vertex/index bytes, and counts. It has no asset
reference or uploaded resource handle. The layout uses the shared value types
in `renderer/core/ShaderDataLayout.hh`; no device or renderer instance is needed
to construct the data.

[`GeometryDataBuilder<T>`](../src/engine/starlight/geometry/GeometryDataBuilder.hh)
is an optional typed producer for procedural geometry. Its `GeometryVertex`
concept requires trivially copyable vertices with `T::layout()` returning
`VertexLayout`. It copies vertex bytes, preserving their explicit stride and
member offsets. [`Vertex3`](../src/engine/starlight/geometry/Vertex.hh) is one
CPU vertex definition; other supported definitions can supply their own layout.
Callers can also populate `GeometryData` directly with bulk bytes and counts.

The same value is accepted by `AssetSystem::createGeometry()` and
[`RendererProxy::createPrimitiveAsync()`](../src/engine/starlight/renderer/RendererProxy.cpp).
The proxy copies it into the owned renderer command. Source data can be modified
or discarded after the call. `GeometryAsset` retains the uploaded handle rather
than keeping a permanent CPU copy; source-data retention belongs to the caller.
Draw selections and GPU-backed primitive handles remain renderer concepts.

## Validation and creation

[`RendererResourcePool`](../src/engine/starlight/renderer/frontend/RendererResourcePool.cpp)
validates uploads before allocating device buffers:

- A nonempty single-stream layout with explicit stride and per-attribute
  location, type, and byte offset. Only `f32_2x1`, `f32_3x1`, and `f32_4x1`
  are supported; other types are rejected before buffer allocation.
- Unique locations in `0..15`, a positive stride of at most 2048 bytes, and
  four-byte-aligned stride/offsets. Each attribute must fit within the stride.
- Positive vertex/index counts, with complete triangles.
- Vertex bytes matching the explicit layout stride and vertex count, including
  padding.
- Exactly `indexCount` native `u32` indices, all below `vertexCount`.

Validation runs in order: layout and counts, byte sizes, then index bounds.
Byte-size checks avoid multiplying an untrusted vertex count by the stride.
Indices are read with `memcpy`, without alignment assumptions about byte storage.
[`VertexLayout`](../src/engine/starlight/renderer/core/ShaderDataLayout.hh)
describes physical storage independently of the shader's required location/type
pairs. Frame acceptance checks that every required input is supplied with the
same type; additional geometry attributes are allowed. Attribute array order
does not determine byte offsets or locations.

[`Vertex3::layout()`](../src/engine/starlight/geometry/Vertex.cpp) uses
`sizeof(Vertex3)` and `offsetof` for position/normal `float3` fields and `float2`
texture coordinates. The upload builder requires trivially copyable vertex
types; their byte size must match the explicit stride. It copies struct bytes
without repacking; callers must describe the member offsets correctly.

Buffer creation errors preserve their original error categories and messages.
Scope guards release created buffers if the second allocation, metadata copying,
or primitive registration fails. Ownership transfers to the primitive pool after
both buffers and their record are created.

## Draw sections

[`PrimitiveDraw`](../src/engine/starlight/renderer/core/Primitive.hh) pairs a
non-owning primitive handle with an optional `IndexRange`. `RenderItem` and
`MaterialDraw` store these selections; the request builder copies them into the
owned request. Omitting the range draws the whole primitive:

```cpp
.primitives = {{.primitive = handle}}
// Select the second triangle from the same upload:
.primitives = {{.primitive = handle, .indexRange = IndexRange{3, 3}}}
```

`firstIndex` and `indexCount` are measured in index elements, not bytes.
[`Primitive::resolveIndexRange()`](../src/engine/starlight/renderer/frontend/RendererResourcePool.cpp)
requires a positive count, both values divisible by three, and a range inside
the uploaded index count. Bounds checks use subtraction to avoid overflow.
[`Renderer::submit()`](../src/engine/starlight/renderer/frontend/Renderer.cpp)
rejects invalid ranges before accepting a frame. Explicit zero counts are
invalid; they do not mean the whole primitive. Indices still address the full
uploaded vertex array; range selection does not rebase them.

Different material draws can reference different ranges of one primitive.
Selections do not allocate or own buffers. The renderer resolves each range and
passes an `IndexedDrawDescription` with named `indexCount`, `firstIndex`, and
`instanceCount` fields through `RenderPassEncoder::drawIndexed()`. The first index
defaults to zero and the instance count to one. Metal adds the first index's byte
offset to the bound index-buffer offset.

## Release and accepted frames

The [asset store](../src/engine/starlight/asset/AssetStore.hh) requests release
from `update()` after the last caller reference disappears. Pending creations
remain tracked until their result can be collected and released.

[`Renderer::destroyPrimitive()`](../src/engine/starlight/renderer/frontend/Renderer.cpp)
drains accepted pending frames, waits for GPU idle, then destroys both buffers
and removes the primitive record. Later submissions with that handle fail
acceptance validation; repeated destruction has no effect. This is the same
blocking retirement approach used for shaders, textures, and samplers.

## Checks

[`AssetSystem.test.cpp`](../src/engine/tests/asset/AssetSystem.test.cpp) checks
owned upload bytes, readiness, last-reference cleanup, shutdown, invalid uploads,
partial allocation errors/exceptions, stale handles, rejected creation commands,
and accepted-frame release ordering. The
[`allocation tests`](../src/engine/tests/asset/AssetAllocation.test.cpp) inject
allocation failures through geometry registration and verify that every accepted
creation is eventually released.

Reflection tests cover float2/3/4 inputs. Geometry upload tests check mixed packed
attributes and rejection of unsupported formats/invalid layouts before allocation.
Renderer tests check shader input subsets, incompatible inputs, and separate
cached pipelines for different physical layouts. Range tests cover invalid and
overflowing selections, two materials/views sharing one buffer pair, copied
request selections, and accepted sections finishing before primitive retirement.
[`MetalGeometry.test.cpp`](../src/engine/tests/renderer/MetalGeometry.test.cpp)
checks fallible native format conversion and renders padded float3/float2/float4
attributes with nonconsecutive locations through the production Metal descriptor
helper into a pixel for readback. It selects differently colored indexed sections
with both a nonzero buffer binding offset and first index. The rendering test
skips without a Metal device.
