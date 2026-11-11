# Shader pipeline

This node records implemented behavior, under the
[knowledge maintenance policy](README.md#maintaining-knowledge-nodes): no ADRs;
update descriptions from code as development changes it.

## Compilation and loading

[`bin/compile_shaders.sh`](../bin/compile_shaders.sh) compiles Slang sources in a
supplied directory to `metallib` on macOS or SPIR-V (`spirv_1_5`) on Linux/the
script's Windows shell branches, with adjacent `<compiled-file>.json` reflection.
The script explicitly selects column-major matrix layout to match GLM.
It is separate from Makefile builds. Metal compilation requires the Xcode Metal
Toolchain; script target branches do not establish engine backend support.

[`AssetSystem`](../src/engine/starlight/asset/AssetSystem.cpp) resolves shader
paths under the configured assets directory.
[`ShaderLoader`](../src/engine/starlight/asset/loaders/ShaderLoader.cpp) reads
compiled bytes and reflection into
[`ShaderDescription`](../src/engine/starlight/renderer/core/Shader.hh). The
renderer receives that value through a command, independently of asset wrappers.

Loading uses [`kc::FileSystem`](../external/kcore/include/kcore/io/FileSystem.hh)
for path checks and binary/text reads, and kcore's JSON parser for reflection.
Missing files and invalid JSON return kcore error codes; shader reflection layout
errors use Starlight's `shaderParsingError` category entry.

The loader delegates decoded JSON to the internal
[`ShaderReflection::parse()`](../src/engine/starlight/asset/loaders/ShaderReflection.cpp)
function. The parser performs no file I/O and returns a dedicated
[`ShaderReflection`](../src/engine/starlight/asset/loaders/ShaderReflection.hh)
value containing stages, vertex layout, uniforms, and resource tables. The loader
uses `and_then` for reading, JSON decoding, and reflection parsing, then
`transform` to combine the reflection with owned bytecode into a complete
`ShaderDescription`. Its parsing helpers handle entry points, vertex inputs,
parameter blocks, and resource arrays independently. JSON access/conversion
errors are translated at the parser entry point; semantic checks return `Result`
errors. Allocation failures are not classified as invalid reflection.

`entryPoints` is required and must be an array. `globalScope` and its `parameters`
may be absent, but must be an object and array respectively when present.
Unknown fields are ignored. Bound vertex inputs require three/four-element
`float32` vectors; unbound inputs are skipped. Numeric layout values must be
non-negative integers within `u32` range; fractional and oversized values are
rejected. Parameter-block names must be nonempty and unique, and entry-point
names must be nonempty.

## Reflection and supported blocks

The loader stores one named entry point per stage. Vertex inputs with bindings
become a list of types in reflected array order. Numeric locations and semantic
names are not retained; only `float32` vectors of three/four elements are parsed.

Global parameters support constant buffers and parameter blocks containing
ordinary data, or blocks containing arrays of sampled 2D textures and samplers.
The loader validates names, counts, and nested layout fields and rejects
unsupported/malformed resource layouts as `shaderParsingError`. Arrays of
ordinary uniform values and blocks mixing data with resource arrays are not
supported by this parser.

[`ShaderUniform` and `ShaderResourceTable`](../src/engine/starlight/renderer/core/ShaderDataLayout.hh)
retain reflected sizes/locations, including resource array counts, offsets,
strides, and bindings. These values belong to the compiled target: the same
Slang shader produces Metal byte offsets/buffer slots and different SPIR-V
descriptor bindings/groups. They are not interchangeable layouts.

Missing resource-array byte offsets, strides, and descriptor indices default to
zero. Descriptor-only layouts can have zero uniform byte size; Metal-specific
layout/residency checks remain in the backend. The
[`parser tests`](../src/engine/tests/asset/ShaderReflection.test.cpp) exercise
these Metal/SPIR-V differences directly from JSON, while the
[`loader test`](../src/engine/tests/renderer/Textures.test.cpp) covers files,
bytecode, JSON decoding, and reflection together.

Callers provide named `ShaderParameterBlock` byte arrays, matched against the
shader's reflected names and sizes. They must pack bytes for the compiled
target. `LayoutHelper` sums value sizes; it is not a portable uniform-layout
packer. `ShaderParameterBlock::fromValue(name, value)` copies a trivially copyable
C++ value into an owned block; it does not repack fields or transpose matrices.
The demo's padded four-`uint` draw block is 16 bytes, and its separate transform
block containing one `float4x4` is 64 bytes on Metal and SPIR-V.

## Transform matrices

The sandbox supplies a GLM `Mat4f` as the named `transform` block:

```cpp
auto block = ShaderParameterBlock::fromValue("transform", modelToClip);
```

[`simple.slang`](../assets/shaders/simple.slang) declares a
`ConstantBuffer<TransformParameters>` with a `float4x4 modelToClip` member and
computes `mul(transform.modelToClip, float4(position, 1.0))` in the vertex shader.
GLM stores matrix columns contiguously, matching the explicit shader compilation
layout. No CPU transpose or backend-specific transform command is involved.
The sandbox replaces the matrix block each frame with a rotation built from
elapsed time. Frame submission owns a copy, so subsequent updates do not mutate
already accepted requests. UV coordinates remain based on object-space position.

This path establishes 4x4 matrix transfer. Other aggregate layouts still need
caller-supplied padding compatible with target reflection; there is no automatic
uniform packing, camera system, or separate model/view/projection API.

## CPU materials and draw preparation

[`ShaderRef::interface()`](../src/engine/starlight/asset/Shader.cpp) queries its
currently published shader through
[`RendererProxy::getShaderInterface()`](../src/engine/starlight/renderer/RendererProxy.cpp).
The proxy queries the shader on the render thread and blocks the caller until
the query completes. Each call builds an owned
[`ShaderInterface`](../src/engine/starlight/renderer/core/Shader.hh) snapshot:
the concrete shader handle and ordinary parameter block names/sizes. Native
binding locations and resource-table layouts remain in the backend description.
The snapshot neither retains the GPU shader nor follows asset shader updates.
An empty shader reference returns an invalid-argument error; initial creation
reports not-ready until published by the asset update. Pending or failed
replacement keeps the previous shader available. A successful publication makes
subsequent queries return the new handle and layout; callers rebuild materials
to capture it. The lower-level proxy query remains available for directly
managed renderer shaders.

[`Material`](../src/engine/starlight/renderer/shading/Material.hh) is a CPU value
that captures this interface and a `ShaderParameters` value: named blocks plus
explicit texture/sampler dependencies. Construction validates the supplied
blocks against the captured layout. Supplied blocks are material-owned; the
remaining shader blocks must be provided by each draw. Names do not imply scope:
the demo's `draw` texture-index block belongs to its material, while `transform`
is supplied per draw.

[`MaterialBuilder`](../src/engine/starlight/renderer/shading/MaterialBuilder.hh)
provides fluent construction from an owned shader-interface snapshot. It adds
named blocks and explicit texture/sampler dependencies, then delegates `build()`
to `Material::create()` for validation. Each build copies an independent material
value; extending the builder does not change earlier materials. The typed
`addParameter()` overload uses `ShaderParameterBlock::fromValue()` without
repacking data. Dependency methods do not write shader indices into blocks.

Materials expose read-only access to their captured interface and parameters;
they do not accept or compose draw inputs.

[`RenderRequestBuilder`](../src/engine/starlight/renderer/frontend/RenderRequestBuilder.hh)
collects views, a clear color, and `MaterialDraw` descriptions through fluent
`addView()`, `setClearColor()`, and `addDraw()` calls. The builder owns the
primitive handles, draw parameter bytes, and dependency declarations it collects;
changing or destroying the original draw description does not change them.
Only the material is borrowed. Its owner must keep it alive and at a stable
address through every `build()` using that draw, including later builds if the
builder is retained. Material access must not race with modification/destruction.

`addDraw()` only collects inputs. `build() const` reads current material values,
combines them with the collected draw blocks, and returns an owned
`Result<RenderRequest>`. It rejects unknown names, wrong sizes, missing blocks,
and duplicate names, including attempts to replace a material-owned block.
Validation is at block granularity; it does not inspect fields, pack bytes,
or infer resource use. Material and draw declarations must include every
resource their bytes can select, including indirect candidates.

Block validation is shared with material construction through
[`validateShaderParameterBlocks()`](../src/engine/starlight/renderer/core/Shader.cpp).
Material construction validates its own subset; building a request additionally
requires every shader block to be supplied. A failed build returns no partial
request and leaves the builder inputs unchanged. Builds can be repeated without
consuming the builder. A material value replaced before a later build is read
again; previously built requests remain unchanged and contain no material refs.

[`RenderRequestBuilder::build()`](../src/engine/starlight/renderer/frontend/RenderRequestBuilder.cpp)
combines material, draw, and frame dependencies, deduplicating complete handles
including their generations. An optional existing request can seed the builder;
its raw items are preserved and keep their render-thread block validation.
Dependency merging uses temporary hash sets while preserving first-seen order
in the returned vectors. Layout/block name checks also use temporary sets.
The resulting request owns all bytes and dependencies and uses the existing
submission path; there are no material commands or GPU material handles.
Request assembly belongs to the renderer frontend; the material module has no
dependency on render requests. A successful build does not prove GPU resource
liveness or renderer acceptance; render-thread validation still applies.

Materials do not own shaders, textures, samplers, or primitives. External owners
must keep handles usable for subsequent submissions. Backend validation still
checks live resources and actual bindings. Previously accepted frames retain
the existing texture/sampler retirement guarantee, not an extended shader or
primitive lifetime guarantee. Replacing a material value affects subsequent
preparation; already prepared requests remain unchanged. Using a replacement
shader requires constructing a material with that shader's interface.

## Mixed resource binding

[`simple.slang`](../assets/shaders/simple.slang) declares a
`ParameterBlock<Resources>` with arrays of 64 textures and 64 samplers, and a
separate `ConstantBuffer<DrawParameters>`. Draw parameters select table entries
by logical resource index. This is bounded indexed resource access, rather than
an unlimited descriptor heap. CPU materials prepare these same blocks and
dependencies; they do not change native resource-table binding.

[`RenderItem`](../src/engine/starlight/renderer/frontend/RenderScene.hh) holds
named parameter bytes. The whole frame declares texture/sampler dependencies;
see [Renderer messaging](renderer-messaging.md) for retention and caller duties.
Textures are immutable, tightly packed linear RGBA8 2D images with one mip level;
samplers independently select nearest/linear filtering and repeat/clamp addressing.
There is no image-file loader or texture render-target implementation.

[`MetalRenderPassEncoder`](../src/engine/starlight/platform/metal/MetalRenderPass.cpp)
coordinates resource validation, uniform binding, and resource-table binding
through standalone helpers in its implementation file. They validate block
counts, names, sizes, binding collisions, and table bounds, then write native
texture/sampler resource IDs into a fresh table byte snapshot using reflected
offsets/strides. Parameter and table bytes are copied through
Metal's `setVertexBytes`/`setFragmentBytes` at reflected buffer slots. Blocks are
limited to 4096 bytes; table counts are at most 64. Only declared textures are
marked resident for vertex/fragment reads. Indirect accesses to undeclared
resources violate the submission contract.

This path requires a Metal 3 device. Vertex geometry uses reserved Metal buffer
slot 30, the last Metal buffer binding, so Slang's low reflected shader slots
cannot overwrite geometry. `metalVertexBufferIndex` names that reservation and
`metalMaxInlineBufferSize` names the inline-byte limit. Binding locations,
resource IDs, and residency calls stay in the Metal backend. SPIR-V compilation
and reflection parsing do not constitute a working Vulkan backend.

## Device shaders and the triangle

[`MetalShader`](../src/engine/starlight/platform/metal/MetalShader.cpp) loads a
Metal library and resolves functions by reflected entry-point name. Its copied
description supplies layouts. Graphics pipelines require vertex/fragment stages,
use packed reflected vertex attributes, and currently support only BGRA8 color
outputs. The renderer verifies primitive/shader vertex layout equality and
propagates buffer-binding errors before drawing.

[`Vertex3`](../src/engine/starlight/renderer/core/Vertex.hh) still has position and
normal `float3` fields. The [sandbox](../src/sandbox/main.cpp) preserves its
existing triangle data; the shader's second input remains named `color` but is
unused. UV coordinates are generated from position. The sandbox uploads a
checkerboard from [`asset/Texture.cpp`](../src/engine/starlight/asset/Texture.cpp),
creates a sampler, and puts both indices and dependencies in one CPU material.
Two instances of the same primitive share that material and supply separate
rotating, translated/scaled matrices in `transform`.

## Engine-thread asset ownership

[`AssetSystem`](../src/engine/starlight/asset/AssetSystem.cpp) centrally stores
shader, texture, and sampler records.
[`ShaderRef`](../src/engine/starlight/asset/Shader.hh),
[`TextureRef`](../src/engine/starlight/asset/Texture.hh), and
[`SamplerRef`](../src/engine/starlight/asset/Sampler.hh) are strong, copyable
references to those records. `ShaderAsset` owns shader loading/reloading behavior;
`TextureAsset` owns texture creation/replacement behavior. Each receives a value
`RendererProxy` for submitting commands. Sampler initialization queues creation
through an injected proxy. References forward resource-specific
operations to their asset object. The store constructs references independently
of `AssetSystem`. References preserve logical asset identity across
renderer-resource replacement. All asset operations, reference copies, and
reference destruction belong to the constructing engine thread.

`loadShader()` reads files and parses reflection synchronously, then queues
renderer creation. Cache keys remove `.` components while preserving `..`, so
parent traversal through symlinks retains its filesystem meaning. The supplied
path is used unchanged for binary/reflection loading and reloads. Repeated loads
with the same cache key share the registered record, including pending or failed
creation. The weak path index does not retain assets. `createTexture()` accepts complete
`TextureUploadData`; separate calls create distinct assets.
`createSampler()` accepts `SamplerDescription` and creates an immutable sampler
asset. Each call creates a separate logical asset, including identical
descriptions. Textures and samplers have independent lifetimes; material data
pairs their handles/indices, and one sampler can be declared by multiple materials.
File reads remain synchronous; image-file decoding is absent.

[`AssetRef`](../src/engine/starlight/asset/Asset.hh) exposes `handle()`,
`pending()`, `version()`, and `error()`. `handle()` returns a `Result`: empty
references are invalid, initial creation is `assetNotReady` until published,
and failed initial creation returns its error. `version()` starts at zero and
increments on every successful publication. `pending()` remains true until
`AssetSystem::update()` collects the renderer future, even if the render thread
has already completed creation. Per-asset creation errors are available on the
reference; `update()` reports errors submitting retirement commands.

`waitForPending()` blocks the engine thread until already queued renderer
creation/replacement commands complete, publishes their results through
`update()`, and waits for requested releases to execute. References can remain
alive during this wait. Per-asset creation errors remain on their references;
the wait reports renderer-command/cleanup errors. It also drains earlier accepted
frames through renderer flushes; it is not a general GPU-completion fence.
Thread checks use `log::expect` in Debug and Release builds.

`ShaderRef::reload()` rereads the original binary/reflection path.
`TextureRef::replace()` supplies all new pixels and dimensions; it allocates a
new renderer texture rather than updating or resampling the old one. Both reject
overlapping operations with `assetBusy`. The old handle/version stays usable
while replacement is pending and after a failed replacement. Successful
publication updates all references to the new handle/version and queues the old
resource for retirement. A failed record can be retried through the same
resource-specific operation; loading its shared path alone does not retry it.

[`AssetRecord`](../src/engine/starlight/asset/Asset.hh) and
[`AssetStore`](../src/engine/starlight/asset/AssetStore.hh)
share future polling, publication, reference tracking, and cleanup across all
three resource types. `AssetRecord` keeps lifecycle state private and exposes
read-only status plus operations for polling and retirement. Resource-specific
operations register their renderer future with `adoptCreationFuture()`. The store is
typed by both asset and reference type: `create()` constructs and registers the
record before calling its private initializer. Constructors and initializers
are private to the stores; callers obtain registered references. Failed initial
setup rolls back empty records. The static reference factory rewraps cached
records, and its weak counterpart supplies the shader path index. Shader cache
storage is allocated before starting creation; assigning the resulting weak
reference requires no allocation. `AssetSystem` uses
explicit steps to update stores, wait for pending commands, and complete shutdown.
Updates attempt every store and every record even after a release submission
fails, then return the first error. Store errors are ordered shader, texture,
then sampler. Expired shader paths are pruned after those attempts.
When the store holds the record's last shared reference, `update()` retires its
renderer resources. `update()` requests release;
reference destructors do no renderer work. Records whose creation is still
pending remain registered until its result can be collected and released.
The store reads `nextRetiredHandle()`, submits its renderer release, and calls
`confirmReleaseSubmitted()` only after command acceptance. Failed submissions
retain their handle for a subsequent update. GPU retirement remains the
renderer's responsibility.
Before consuming a successful creation future, polling reserves space for the
previous handle's retirement. If allocation fails, the future and current
handle remain available for retry. Allocation exceptions propagate to the caller;
accepted creation remains owned by the store.
The asset system has no private polling thread or per-resource locks.

Renderer handles and CPU materials remain non-owning snapshots. Callers retain
asset references through request preparation and submission. After asset
replacement, callers must explicitly rebuild materials/parameter bytes and
unsubmitted requests that captured the previous handles; `version()` allows
callers to detect publication. Renderer requests contain no asset references.
Accepted frames are protected by renderer retirement, not CPU reference counts.

`shutdown()` requires every asset reference to be released, otherwise it returns
`assetInUse`. With the renderer still running, it uses `waitForPending()` to
collect queued creation results, release remaining resources, and wait for release
commands to execute. Successful shutdown is idempotent and rejects subsequent creation;
`update()` after shutdown does nothing. The destructor calls shutdown and
terminates with an error diagnostic if the lifetime contract is violated or
cleanup fails. Destroy scene/object references first, then assets, then rendering.
The [sandbox](../src/sandbox/main.cpp) keeps shader, texture, and sampler
references alive alongside its material, calls `waitForPending()` at startup, pumps `update()`
each frame, and releases shader, texture, and sampler references before the
renderer is destroyed.

## Checks

[`AssetSystem.test.cpp`](../src/engine/tests/asset/AssetSystem.test.cpp) uses a CPU
test device with the real renderer executor/command path to check path sharing,
symlink/parent traversal during loading and reload,
publication, replacements and errors, independent sampler/texture lifetimes,
last-reference cleanup, pending creation cleanup, failed retirement retry,
progress of later records after release rejection, and shutdown. It also checks pending-frame shader retirement, matching
pipeline cleanup, and stale shader rejection.
Compile-time checks reject direct construction of shader, texture, and sampler
implementations. The separate
[`allocation tests`](../src/engine/tests/asset/AssetAllocation.test.cpp) isolate
the allocation injector from other executables. They check replacement retry,
shader-cache setup failure, and allocation points through texture/sampler
creation, verifying that every accepted creation is released at shutdown.

[`Materials.test.cpp`](../src/engine/tests/renderer/Materials.test.cpp) covers
material/draw block validation, dependency union, and independent owned draw
snapshots. The shader-interface test in
[`Textures.test.cpp`](../src/engine/tests/renderer/Textures.test.cpp) checks the
command reply's copied metadata and invalid handles.
[`Textures.test.cpp`](../src/engine/tests/renderer/Textures.test.cpp) covers CPU
checkerboard output, owned column-major matrix copies, pending-frame retirement
order, and Metal/SPIR-V-style
reflection parsing and malformed-input rejection.
[`MetalTextures.test.cpp`](../src/engine/tests/renderer/MetalTextures.test.cpp)
uses a small independent MSL fixture to sample a checkerboard offscreen through
the production table encoder at nonzero indices and reject stale handles after
slot reuse. It uses CPU material preparation for its parameters and dependencies
and also binds a translated/scaled GLM matrix in the vertex stage
and checks both covered and cleared pixels to detect matrix-layout mistakes.
It skips when no Metal 3 device is accessible; it does not exercise
the sandbox's Slang-to-metallib compilation.
