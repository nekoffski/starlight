# Shader parameters and resource binding

Research date: 2026-09-29. This is a proposed direction, not an implemented renderer feature. The vertex-layout proposal in [renderer-design.md](renderer-design.md) remains the geometry interface. This note covers ordinary shader data and resources for Metal and Vulkan.

## Recommendation

Use a **hybrid interface**: reflected parameter groups for frame/pass data and explicit pass resources, ordinary buffers for matrices/colors/flags and object/material records, and indexed texture/sampler tables for the scalable material path. Keep the frontend independent of native descriptors and GPU addresses.

Bindless means that a shader selects a resource from a table or handle rather than requiring the CPU to bind that particular resource for every draw. It does not replace the need to upload matrices, define their memory layout, synchronize writes, or retain resources. Metal's bindless mechanism uses argument buffers; Vulkan descriptor indexing uses descriptor arrays. [Apple bindless overview](https://developer.apple.com/videos/play/wwdc2021/10286/), [Khronos descriptor indexing](https://docs.vulkan.org/guide/latest/extensions/VK_EXT_descriptor_indexing.html)

Do not make “every uniform is a bindless resource” the contract. An object table containing thousands of matrices is one buffer, indexed by object ID; it need not allocate thousands of buffer descriptors. Keep an efficient direct constant-buffer path as well: Khronos' constant-data sample demonstrates that indexed storage-buffer data and push constants are not universally faster than directly bound uniform data, particularly on the Mali hardware it tested. Those results are hardware-specific, not a benchmark of Starlight. [Khronos constant-data sample](https://docs.vulkan.org/samples/latest/samples/performance/constant_data/README.html)

## What existing engines teach

- **Bevy:** materials can opt into bindless textures/samplers, receive indices into arrays separated by type, and fall back to conventional bindings on unsupported platforms. Its bindless material ABI uses an index table plus material data; slabs have bounded, device-dependent capacity. This is evidence for hybrid binding and bounded tables rather than insisting on a single unlimited universal heap. [Bevy AsBindGroup documentation](https://docs.rs/bevy/latest/bevy/render/render_resource/trait.AsBindGroup.html), [bindless material example](https://github.com/bevyengine/bevy/blob/main/examples/shader/shader_material_bindless.rs)
- **Filament:** material parameter names resolve through the material's interface into descriptor bindings. Its implementation validates texture usage and compatibility. Copy the separation between user-facing named material parameters and native binding operations, including validation; do not expose backend binding indices to gameplay code. [Filament material-instance implementation](https://github.com/google/filament/blob/main/filament/src/details/MaterialInstance.cpp)
- **Daxa:** a deliberately Vulkan-focused design uses bindless resource handles throughout. It is a useful counterexample showing how much a native Vulkan engine can simplify by choosing a strong device/API baseline. Its claims of simpler descriptor management are not evidence that the same API or performance holds on Metal. [Daxa bindless design](https://wiki.daxa.dev/bindless/)

The shared principle to adopt is stable logical data/resource interfaces with backend-owned binding and lifetime rules. There is no single best binding model for every renderer and device.

## Shader authoring: use Slang parameter blocks

Slang's `ParameterBlock<T>` groups ordinary values and resources; Vulkan receives a descriptor set, while Metal receives an argument buffer. Slang recommends grouping parameters by update frequency and declaring less frequently changed blocks earlier. For raster pipelines it recommends global parameter blocks shared by the stages used together. Ordinary fields in a Vulkan block introduce a uniform buffer automatically. [Slang parameter-block guide](https://docs.shader-slang.org/en/latest/parameter-blocks.html)

Proposed group roles:

| Logical role | Examples | Typical update |
| --- | --- | --- |
| Frame/view | view-projection matrix, camera position, time | Per frame or view |
| Pass | depth texture, shadow map, lighting buffers | Per pass |
| Material | tint, roughness, texture/sampler references | On material change |
| Object/draw | model matrix, object/material indices, flags | Per object/draw |

These are frontend roles, not hard-coded Vulkan set numbers or Metal buffer indices. A particular shader can use only the groups it needs. Their target-specific reflected layouts form the actual program ABI. A global renderer convention can keep common blocks compatible across programs, but changing a declaration must still trigger layout revalidation and pipeline regeneration.

Use `ConstantBuffer<T>` for an explicitly ordinary-data buffer, `StructuredBuffer<T>` for indexed records, and `ParameterBlock<T>` for a reusable collection of data/resources. Avoid one parameter block per scalar or per instance. Do not conflate a table of object records with a descriptor array of buffers.

### Proposed portable frontend seam

```text
Shader artifact + target reflection -> immutable ProgramLayout
ProgramLayout + user values/resources -> validated ParameterGroup
ParameterGroup + frame upload allocation -> backend binding representation
Render pass binds ParameterGroup by logical group identity
Draw selects object/material record using a small draw-parameter block
```

The parameter group should carry ordinary data plus typed resource references. A buffer reference includes its slice (resource, offset, range); a texture reference includes the view/type; a sampler reference includes the sampler handle. Backend lowering writes Vulkan descriptors or Metal argument-buffer fields. CPU-owned resource handles never become native Metal IDs or Vulkan heap indices by simple copying.

Resolve names to a layout-qualified field/group ID when building a material or draw packet. Perform type/range validation there, then record draws using IDs and handles. A field ID must carry or be checked against the shader/layout generation so hot reload cannot reuse an old field index with a different program. The layout is immutable and reusable; the material/group values are mutable and must be captured into a frame-safe version before submission.

Initially, the renderer can support a compact reflected setter for scalar/vector/matrix fields and typed resources. Typed engine blocks may use raw bytes only when their offsets, sizes, array strides, and matrix representation are verified against the selected target reflection. A generic string lookup for every parameter on every draw is unnecessary.

## Ordinary data: packing is part of the shader ABI

Uniform values include matrices, vectors, colors, scalars, and flags. Values
shared across shader invocations belong in constant blocks or indexed records;
per-vertex positions/colors still use the geometry interface. Bindless changes
resource selection, not the representation of these ordinary values.

### Local compiler observations

The following checks used the installed `slangc 2026.17`, with an explicit
`-matrix-layout-column-major`, on 2026-09-29. A `ConstantBuffer<FrameData>`
containing the fields below was compiled to Metal and SPIR-V. The reflection
reported different layouts even though the Slang source was identical:

| Field, in declaration order | Metal offset / size | SPIR-V offset / size |
| --- | --- | --- |
| `float4x4 viewProjection` | 0 / 64 | 0 / 64 |
| `float3 eye` | 64 / 16 | 64 / 12 |
| `float time` | 80 / 4 | 76 / 4 |
| `float weights[3]` | 84 / 12; stride 4 | 80 / 48; stride 16 |
| `float3x3 normalMatrix` | 96 / 48 | 128 / 48 |
| `uint flags` | 144 / 4 | 176 / 4 |
| `bool enabled` | 148 / 1 | 180 / 4 |
| `float2 uvScale` | 152 / 8 | 184 / 8 |
| Total block | 160 bytes | 192 bytes |

A mixed `ParameterBlock<MaterialData>` containing `float4 color`, `uint flags`,
`Texture2D<float4> albedo`, and `SamplerState surfaceSampler` also compiled.
Metal emitted a struct containing ordinary fields and native texture/sampler
fields, bound through `constant* [[buffer(1)]]`; its resource offsets were 24
and 32, each eight bytes. There were no `[[id]]` member annotations in this
generated representation. SPIR-V reflection placed the block in set 1, with
an implicit ordinary-data buffer at binding 0 and texture/sampler at bindings
1 and 2. The Metal source also passed Apple's offline compiler to produce
`.metallib`. These observations establish compiler output, not runtime
resource encoding or GPU correctness.

This is a concrete reason to keep shader group layouts target-specific.
Blindly copying a C++ struct containing `Vec3f`, scalar arrays, `Mat3f`, or
`bool` is not the general upload interface. Vulkan's documented alignment
models also differ by selected feature/layout policy.
[Khronos shader memory layout](https://docs.vulkan.org/guide/latest/shader_memory_layout.html)

### Proposed packing contract

- Resolve a user field to a layout-qualified ID once. A typed setter checks
  the value type and writes components at the reflected target offsets and
  array strides. Zero padding; do not copy a CPU value using the shader's
  padded size as the source byte count.
- Represent persistent shader flags as `uint32` bitmasks, not C++ `bool`.
  The public interface may accept a logical boolean if it performs explicit
  target encoding, but flags are the simpler shared record representation.
- Engine frame/object/material records should use deliberately specified
  `float4`, `float4x4`, and `uint32` fields, with explicit padding and layout
  checks. This permits a fast raw upload path when both target layouts match.
  The flexible reflected path remains available for user-defined blocks.
- Pin matrix storage and multiplication conventions in the shader build.
  For Starlight's current GLM types, propose column-major storage and
  `mul(matrix, vector)` in Slang. Do not rely on `slangc`'s default. Non-square
  matrices and 3x3 matrices need component-wise packing with padded vectors.
- The tested JSON includes matrix dimensions and total size but omits matrix
  major order and an explicit matrix stride. Record the compiler convention
  in the artifact manifest; initially transfer 4x4 matrices under that
  convention. Before supporting arbitrary matrix layouts or field overrides,
  enrich offline metadata using Slang's reflection API. Do not silently infer
  every matrix layout from the existing JSON.

Slang distinguishes matrix storage from vector/matrix multiplication order
and offers explicit compiler flags. Its reflection API can query matrix
layout mode; it also warns that reflected Slang row/column terms differ from
SPIR-V decoration terminology.
[Slang matrix conventions](https://shader-slang.org/slang/user-guide/a1-01-matrix-layout.html),
[Slang matrix reflection](https://shader-slang.org/slang/user-guide/reflection)

Buffer-slice alignment is a separate requirement from field packing. Vulkan
uniform-buffer base and dynamic offsets must honor
`minUniformBufferOffsetAlignment`; query storage-buffer alignment as well.
Allocate frame slices accordingly instead of hard-coding a universal offset
alignment. Keep the byte range in resource bindings, not just a buffer handle
and offset.
[Khronos dynamic offsets](https://docs.vulkan.org/guide/latest/descriptor_dynamic_offset.html)

### Frontend usage sketch

This is an illustrative interface, not implemented method names:

```cpp
auto frameLayout = shader.layout().group("frame");
auto viewProjection = frameLayout.field<Mat4f>("viewProjection");
auto frame = ParameterGroup::create(frameLayout);
frame.set(viewProjection, camera.viewProjection());

auto materialLayout = shader.layout().group("material");
auto tint = materialLayout.field<Vec4f>("color");
auto flags = materialLayout.field<u32>("flags");
auto material = ParameterGroup::create(materialLayout);
material.set(tint, Vec4f{1.0f, 0.2f, 0.1f, 1.0f});
material.set(flags, u32{1});
material.set(materialLayout.texture("albedo"), albedoTexture);
material.set(materialLayout.sampler("surfaceSampler"), linearSampler);
```

Snapshots of these groups accompany view/material data in the render request.
The caller supplies typed values and resource handles; the renderer packs
ordinary data, chooses upload slices, and lowers resource references into
direct bindings or table indices for the selected shader variant. Layout
lookup and field IDs are independent of Vulkan set numbers and Metal slots.

The RHI can extend its planned `setBindGroup` operation for reusable groups and
provide a small logical draw-data operation. Vulkan uses push constants for
tiny selectors; Metal can copy small values with `setVertexBytes`/
`setFragmentBytes`, or bind a frame slice. Apple's copied-bytes operation is
for single-use data below 4 KB; it is not a license to push an arbitrary whole
material through either backend. Large matrices/record arrays use buffers.
[Apple small-data binding](https://developer.apple.com/documentation/metal/mtlrendercommandencoder/setvertexbytes(_:length:index:))

For the scalable path, a draw carries small object/material indices and flags;
the shader loads transform and material records from shared buffers. Preserve
the direct uniform path for simple draws and passes, and benchmark the table
path rather than promising that an extra indirection improves every workload.

## Bindless material ABI: logical indices first

Prefer engine-owned `uint32` texture and sampler indices in GPU material records, with **separate typed arrays** for 2D textures, cube textures, storage images, and samplers as required. Texture table compatibility includes sampled scalar type and depth/comparison semantics, not just dimensionality. Geometry keeps its existing vertex-buffer interface; vertex pulling can be introduced separately for a GPU-driven workload.

Proposed ordinary record (illustrative, not a finalized packed ABI):

```cpp
struct MaterialRecord {
    float tint[4];
    uint32_t albedoTexture;
    uint32_t albedoSampler;
    uint32_t flags;
    uint32_t reserved;
};
```

Shader helpers resolve these indices through the selected typed table. The Metal path stores native resource IDs in the table; the Vulkan path writes the corresponding descriptor arrays. The index in the ordinary material record can remain identical across backends; the table representation cannot. Bind the table once for a compatible pass/batch, not for every individual texture change.

Choose capacity from supported limits and compile a corresponding shader variant, or use a small supported set of capacities. A fixed bounded table is a valid bindless implementation and avoids requiring runtime arrays or variable descriptor count on the first version. Handle exhaustion explicitly: fail with diagnostics, grow into a supported variant, or use multiple table batches with deliberate remapping. Do not silently truncate or assume “unbounded” means infinite memory.

A fallback material shader can access directly bound textures through the same high-level sampling helper. Select the binding variant when creating the program/material; do not branch between native APIs in per-pixel code.

An important constraint on this fallback: a compiled Slang
`ParameterBlock<T>` containing resources can require a Metal argument buffer.
It cannot be implemented by expanding the same compiled block into independent
`setTexture`/`setSampler` calls. A direct-binding fallback needs a shader
declaration variant that actually emits direct resource arguments. Reusing the
logical frontend group does not imply identical native shader ABIs.

Recommend Metal 3/Tier 2 with macOS 13 or later for the preferred native-layout
path. Supporting older OS versions or tiers requires a deliberately compiled
direct-resource fallback; removing the large table alone does not remove the
native-layout requirement. Distinct shader variants have distinct reflected
layout identities, even when they expose the same logical group/field names.

### Why not standardize on Slang DescriptorHandle bytes?

`DescriptorHandle<T>` is intended to make resources usable as ordinary shader data, but its representation is target-dependent. The current Slang guide describes Vulkan handles as `uint2`, automatic heap generation, and optional `spvDescriptorHeapEXT` lowering. It also documents custom descriptor fetch and a `VkMutable` default preset, with `BindlessDescriptorOptions.None` available for typed bindings. Consequently, default Vulkan handles can require `VK_EXT_mutable_descriptor_type` in addition to indexing support. [Slang convenience-feature source](https://github.com/shader-slang/slang/blob/master/docs/user-guide/03-convenience-features.md)

Current upstream Slang RHI contains a Metal Tier-2 requirement and a residency-set/useResource fallback; Slang's own Metal handle tracking describes native buffer pointers and texture/sampler IDs. Combined texture+sampler handle encoding has had a separately tracked compatibility problem. These are reasons to verify the installed compiler and resource kinds, and to prefer separate textures/samplers for the first portable ABI. An issue description records the problem at its publication time; a closed status alone is not proof that every installed release supports every shape. [Slang RHI Metal device](https://github.com/shader-slang/slang-rhi/blob/main/src/metal/metal-device.cpp), [Metal combined-handle tracking](https://github.com/shader-slang/slang/issues/11540)

Typed Slang handles remain a useful future internal backend mechanism. Do not expose their raw bytes as the public frontend resource handle or persisted asset ABI. `ResourceDescriptorHeap[index]` HLSL-style syntax is also not the recommended portable source interface: its currently documented recovery path excludes Metal. [Slang convenience-feature source](https://github.com/shader-slang/slang/blob/master/docs/user-guide/03-convenience-features.md)

Bindless heap use may not appear in normal parameter enumeration because lowering introduces it later. Current Slang documentation separates the reserved `getBindlessSpaceIndex()` from post-emission `IBindlessResourceMetadata::usesBindlessResourceHeap()`. Record the latter signal and binding model in the shader artifact when using compiler-generated heaps. Do not treat a reserved index as proof that an emitted shader needs a heap. [Slang reflection guide](https://shader-slang.org/slang/user-guide/reflection.html)

## Backend requirements and future paths

| Concern | Vulkan | Metal |
| --- | --- | --- |
| Ordinary data | Uniform/storage buffers; dynamic offsets when appropriate | Constant/device buffer slices |
| Reusable parameter group | Descriptor set/layout | Compiled argument-buffer layout |
| Indexed material textures | Typed descriptor arrays with required indexing features | Tier-2 argument-buffer texture arrays |
| Small draw parameters | Push constants within device limits | Small copied bytes or frame-buffer slice |
| Indirectly referenced resources | Valid descriptors, resource transitions and lifetime | Residency declarations plus hazards/lifetime |

For Vulkan, query and enable the precise indexing features needed: divergent sampled-image indexing needs the corresponding non-uniform feature; runtime arrays, partially-bound arrays, variable descriptor counts, and update-after-bind each have separate requirements. Vulkan 1.2 incorporating descriptor indexing does not mean every optional feature is available. Initially fill unused slots with valid fallback resources and version tables at frame-safe points, avoiding optional update-after-bind and partially-bound behavior until useful. Partially-bound descriptors only excuse unused slots; they do not make an accessed invalid descriptor safe. [Khronos indexing guide](https://docs.vulkan.org/guide/latest/extensions/VK_EXT_descriptor_indexing.html), [Vulkan descriptor-set specification](https://docs.vulkan.org/spec/latest/chapters/descriptorsets.html)

For Metal Tier 2 on macOS 13/iOS 16 and later, Apple documents directly encoding buffer GPU addresses and resource IDs according to the C-compatible argument-buffer layout. Earlier OS/tier paths use `MTLArgumentEncoder`. Choose the method that matches the actual generated shader representation; do not apply an argument encoder designed for a different layout. Indirect resources require residency declarations even when their argument buffer itself is bound. [Apple argument-buffer guide](https://developer.apple.com/documentation/metal/improving-cpu-performance-by-using-argument-buffers)

Residency and synchronization are distinct. `useResource`/`useHeap` or residency sets make allocations available, but do not permit overwriting data still read by the GPU. Heap-level hazard tracking can create false sharing, and opting out requires explicit synchronization. Keep per-pass access information rather than inferring “every bindless resource is writable.” [Apple Metal 3 bindless session](https://developer.apple.com/videos/play/wwdc2022/10101/), [Apple residency-set guide](https://developer.apple.com/documentation/metal/simplifying-gpu-resource-management-with-residency-sets)

### Cutting-edge Vulkan: descriptor heaps

`VK_EXT_descriptor_heap` was announced in January 2026 as a replacement descriptor system. The current extension reference marks it ratified; the launch's request for feedback before a future KHR form should not be read as a current draft status. Investigate it as an optional Vulkan implementation path, not a universal device requirement. Check extension dependencies, the descriptor-heap feature, buffer-device-address support, and heap size/alignment properties. The mapped-binding path still requires support for shader untyped pointers, although that feature need not be enabled when only using the binding interface. [Khronos announcement](https://www.khronos.org/blog/vulkan-introduces-roadmap-2026-and-new-descriptor-heap-extension), [extension requirements and status](https://docs.vulkan.org/refpages/latest/refpages/source/VK_EXT_descriptor_heap.html)

The extension can map existing SPIR-V descriptor-set/binding declarations to heap entries at pipeline creation; it does not require immediately rewriting all shader source to direct heap syntax. The implementation must honor device descriptor sizes/alignment and driver-reserved heap ranges. [Khronos heap guide](https://docs.vulkan.org/guide/latest/descriptor_heap.html), [heap sample](https://docs.vulkan.org/samples/latest/samples/extensions/descriptor_heap/README.html)

Heap mode uses `vkCmdPushDataEXT`, not `vkCmdPushConstants`, and does not coexist with active descriptor-set/buffer state: setting one invalidates the other. Therefore “hybrid” here describes ordinary data plus indexed resources. A heap backend must lower *all* logical parameter groups consistently through heap mappings; it cannot bind old frame descriptor sets beside a new material heap in the same active pipeline state. [Vulkan descriptor-heap specification](https://docs.vulkan.org/spec/latest/chapters/descriptorheaps.html)

The current Vulkan specification presents descriptor heaps as a replacement for legacy descriptor management, also covering descriptor buffers. Do not make `VK_EXT_descriptor_buffer` the mandatory next investment solely because it sounds more modern than sets; assess a heap path instead when supported. Descriptor indexing remains a practical compatibility implementation behind the same logical frontend. [Vulkan legacy/superseded functionality](https://docs.vulkan.org/spec/latest/appendices/legacy.html)

### Metal 4

Metal 4 argument tables are another backend binding mechanism. Apple documents taking a snapshot of table resources when encoding a draw, dispatch, or execute command. They should not be confused with a shader-indexed bindless array; keep logical parameter groups independent of the encoder binding API so a later Metal 4 backend can select its native mechanism. [Apple Metal 4 argument-table binding](https://developer.apple.com/documentation/metal/mtl4rendercommandencoder/setargumenttable(_:stages:))

## Lifetime invariants proposed for Starlight

1. An upload allocation is immutable after a recorded draw references it; reclaim it only after GPU completion. Use frame arenas/rings, not in-place writes to the same matrix for successive pending draws.
2. A submitted parameter group retains its referenced resources and table version through completion. Shader layout and material edits create new versions for future work.
3. A bindless slot is not recycled until live CPU material/record references are removed or remapped and every submission that could reference its old contents completes. CPU generation counters catch stale CPU handles; GPU indices additionally need deferred recycling or explicit GPU generation validation. A table version and the material indices selecting it must agree; switching batches/slabs requires deliberate record remapping.
4. Slot zero can be a permanent fallback per table type. Validate missing resources and index bounds; avoid relying on invalid/native null descriptors to produce a color.
5. Track read/write resources independently of how they were bound. A bindless shader can select resources dynamically; require explicit pass resource declarations or a conservative live set where precise access cannot be known.
6. For frame-versioned tables, upload changes to the current reusable frame's table and propagate them to the other versions at their safe points. A single shared table requires a stricter concurrent-update scheme.

These proposed rules follow the synchronization restrictions in the descriptor-set specification and Metal's residency/hazard requirements above. They are not yet guarantees of Starlight.

## Validation before calling this ready

### Bounded-table compiler probe

A local probe used a `ParameterBlock` containing 64 `Texture2D<float4>` values
and eight samplers, a structured material-record buffer, and a flat fragment
material index. Material records contain ordinary `uint` table indices. The
following source pattern passed Slang generation for both targets:

```slang
[ForceInline]
uint tableIndex(uint index) {
    __target_switch {
    case spirv: return NonUniformResourceIndex(index);
    default: return index;
    }
}

struct ResourceTable {
    Texture2D<float4> textures[64];
    SamplerState samplers[8];
};
struct MaterialData {
    float4 color;
    uint textureIndex;
    uint samplerIndex;
    uint flags;
};
ParameterBlock<ResourceTable> resources;
StructuredBuffer<MaterialData> materials;

[shader("fragment")]
float4 fragmentMain(float2 uv : TEXCOORD0,
                    nointerpolation uint materialIndex : TEXCOORD1) : SV_Target {
    MaterialData material = materials[materialIndex];
    return resources.textures[tableIndex(material.textureIndex)]
        .Sample(resources.samplers[tableIndex(material.samplerIndex)], uv)
        * material.color;
}
```

Save this block as `probe.slang` and reproduce with:

```sh
slangc probe.slang -target metal -o probe.metal -reflection-json probe.metal.json
slangc probe.slang -target spirv -profile spirv_1_5 -o probe.spv -reflection-json probe.spv.json
slangc probe.slang -target metallib -o probe.metallib
```

For this fragment probe, the following minimal binary check fails on the
observed non-inlined wrapper variant:

```sh
python3 - <<'PY'
import struct
data = open('probe.spv', 'rb').read()
words = struct.unpack('<' + 'I' * (len(data) // 4), data)
capabilities, decorations = set(), set()
i = 5
while i < len(words):
    count, opcode = words[i] >> 16, words[i] & 65535
    args = words[i + 1:i + count]
    if opcode == 17: capabilities.add(args[0])  # OpCapability
    if opcode == 71: decorations.add(args[1])  # OpDecorate
    i += count
assert {5301, 5307} <= capabilities
assert 5300 in decorations
print('Non-uniform indexing metadata present')
PY
```

This check establishes metadata presence for the probe. A full validation
must also inspect the decorated descriptor access path and execute the shader;
decorating an unrelated value is not sufficient.

The local Apple compiler also produced `.metallib` for this pattern. The
capacities above are probe sizes, not selected engine limits. SPIR-V declared
`ShaderNonUniform` (5301), `SampledImageArrayNonUniformIndexing` (5307), and
`NonUniform` decorations (5300), without a runtime descriptor array.
Metal emitted native resource arrays with eight-byte element strides.

There were two failed variants worth retaining as regression cases:

1. Direct use of `NonUniformResourceIndex` in Metal produced Slang E36107
   (unavailable feature). The target-specific helper avoids that call on Metal.
2. Removing `[ForceInline]` from the integer-returning helper still compiled
   to SPIR-V, but the inspected binary lost the non-uniform decorations and
   indexing capabilities for divergent indices. The direct Vulkan expression
   and forced-inline helper retained them. This is an observation about the
   tested compiler and wrapper shape, not a diagnosis of a general Slang bug.

Inspect emitted SPIR-V after compiler upgrades: require the relevant indexing
capabilities and non-uniform decorations on the descriptor access path when
indices can diverge. Successful source compilation alone is insufficient.

Compile-only success establishes generated shader/reflection shape, not correct GPU execution. Before implementing the scalable path, retain a small shader probe and then run an integration draw on each backend that checks:

- A matrix, nontrivial color, and `uint32` flags reach both shader stages correctly.
- Arrays and nested structs match reflected offsets/strides; matrix order is deliberate.
- Two materials select different textures/samplers without rebinding per texture.
- Divergent resource indexing uses the appropriate generated Vulkan non-uniform decorations.
- Indirect Metal resources are declared resident, with validation enabled.
- Resource destruction/recreation and texture streaming are safe with multiple frames in flight.
- Table-capacity overflow, invalid handles, and unsupported device capabilities produce useful errors.
- The conventional binding fallback gives the same image.

Performance comparisons should measure CPU recording cost and GPU time separately for direct constants, indexed records, and bindless textures on representative target hardware. The modern path should be selected for workload/device evidence; the interface should support that choice without changing material authoring.

## Suggested increments

1. Reflect and bind one frame/object constant block; establish packing and upload lifetime checks.
2. Add a material parameter group with color/flags plus one texture and sampler on both backends.
3. Add typed bounded texture/sampler tables, logical indices, capability selection, and deferred slot recycling. This is the preferred scalable material path when validated.
4. Add structured object/material records and draw indices for batching/GPU-driven workloads, preserving the direct-data path for other passes.
5. Add an optional Vulkan descriptor-heap implementation using the same logical layouts; add Metal 4 binding later if the chosen platform baseline warrants it.

No third-party rendering abstraction is required for these increments. Slang RHI is useful reference code for reflection/binding and runtime tests; adopting it wholesale would replace a significant portion of Starlight's existing RHI and is a separate architectural choice.
