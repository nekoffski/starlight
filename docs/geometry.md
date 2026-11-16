# Geometry assets

[`AssetSystem::createGeometry()`](../src/engine/starlight/asset/AssetSystem.cpp)
accepts `PrimitiveUploadData` and returns a strong
[`GeometryRef`](../src/engine/starlight/asset/Geometry.hh). Creation uses the
existing asynchronous primitive command; the command owns copies of the vertex
and index bytes. `update()` publishes the resulting `PrimitiveHandle`, or exposes
the creation error through the reference. `waitForPending()` provides blocking
startup publication. Geometry assets are immutable after creation.

Copies of a reference retain the same geometry. Independent creation calls make
distinct assets. References expose the shared asset readiness/error/version
interface; they stay on the asset system's engine thread. Retain a reference
through draw preparation and submission, then release it before asset shutdown.
The [sandbox](../src/sandbox/main.cpp) creates its triangle through this path and
shares one primitive across both instances.

## Validation and creation

[`RendererResourcePool`](../src/engine/starlight/renderer/frontend/RendererResourcePool.cpp)
validates uploads before allocating device buffers:

- A nonempty layout containing defined `ValueType` values.
- Positive vertex/index counts, with complete triangles.
- Tightly packed vertex bytes matching the layout stride and vertex count.
- Exactly `indexCount` native `u32` indices, all below `vertexCount`.

Validation runs in order: layout and counts, byte sizes, then index bounds.
Byte-size checks avoid multiplying an untrusted vertex count by the stride.
Indices are read with `memcpy`, without alignment assumptions about byte storage.
This validates the upload's shape; it does not add native vertex formats or
change shader/geometry layout matching.

Buffer creation errors preserve their original error categories and messages.
Scope guards release created buffers if the second allocation, metadata copying,
or primitive registration fails. Ownership transfers to the primitive pool after
both buffers and their record are created.

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
