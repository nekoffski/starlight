# Shader pipeline

This node records implemented behavior, under the
[knowledge maintenance policy](README.md#maintaining-knowledge-nodes): no ADRs;
update descriptions from code as development changes it.

## Compilation and loading

[`bin/compile_shaders.sh`](../bin/compile_shaders.sh) compiles Slang sources in a
supplied directory to `metallib` on macOS or SPIR-V (`spirv_1_5`) on Linux/the
script's Windows shell branches, with adjacent `<compiled-file>.json` reflection.
It is separate from Makefile builds. Metal compilation requires the Xcode Metal
Toolchain; script target branches do not establish engine backend support.

[`AssetSystem`](../src/engine/starlight/asset/AssetSystem.cpp) resolves shader
paths under the configured assets directory.
[`ShaderLoader`](../src/engine/starlight/asset/loaders/ShaderLoader.cpp) reads
compiled bytes and reflection into
[`ShaderDescription`](../src/engine/starlight/renderer/core/Shader.hh). The
renderer receives that value through a command, independently of asset wrappers.

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

Callers provide named `ShaderParameterBlock` byte arrays, matched against the
shader's reflected names and sizes. They must pack bytes for the compiled
target. `LayoutHelper` sums value sizes; it is not a portable uniform-layout
packer. The demo's explicitly padded four-`uint` block is 16 bytes on both targets.

## Mixed resource binding

[`simple.slang`](../assets/shaders/simple.slang) declares a
`ParameterBlock<Resources>` with arrays of 64 textures and 64 samplers, and a
separate `ConstantBuffer<DrawParameters>`. Draw parameters select table entries
by logical resource index. This is bounded indexed resource access, rather than
an unlimited descriptor heap or an implemented material system.

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
creates a sampler, supplies both indices in `draw`, and declares both resources.

[`ShaderRef`](../src/engine/starlight/asset/Shader.cpp) retains its current asset
behavior: an event loop polls asynchronous creation, `waitForDevice()` polls the
queued state, and destruction requests device shader release. References to the
asset proxy/event loop do not extend their lifetimes. `requestUpdate()` replaces
the handle without releasing the previous one or invalidating pipelines. Texture
support does not redesign these asset ownership rules.

## Checks

[`Textures.test.cpp`](../src/engine/tests/renderer/Textures.test.cpp) covers CPU
checkerboard output, pending-frame retirement order, and Metal/SPIR-V-style
reflection parsing and malformed-input rejection.
[`MetalTextures.test.cpp`](../src/engine/tests/renderer/MetalTextures.test.cpp)
uses a small independent MSL fixture to sample a checkerboard offscreen through
the production table encoder at nonzero indices and reject stale handles after
slot reuse. It skips when no Metal 3 device is accessible; it does not exercise
the sandbox's Slang-to-metallib compilation.
