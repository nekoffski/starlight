# Renderer language

These terms describe the implemented renderer. For behavior and source links,
see [Renderer messaging](renderer-messaging.md) and [Shader pipeline](shader-pipeline.md).
Follow the [knowledge maintenance policy](README.md#maintaining-knowledge-nodes):
no ADRs, and update definitions from code during development.

## Language

**Render request**:
A scene, views, and clear color submitted together for rendering.
Acceptance is distinct from completion of the frame on the GPU.

**Render item**:
A selection of uploaded primitives drawn with one shader in a scene.

**Primitive**:
Uploaded vertex and index data with the vertex layout and draw count needed to
draw it. It is distinct from the currently empty asset-level mesh concept.

**Render target**:
The destination selected by a view. Surface destinations are implemented;
the declared texture destination is not implemented.

**Shader description**:
Compiled shader bytes and metadata describing entry points, vertex inputs,
reflected uniforms, and resource tables.

**Shader reference**:
A strong reference to a centrally stored engine-thread shader asset. Copies
share identity; `reload()` replaces its renderer backing after success. The
reference exposes readiness, errors, a fallible handle, and publication version.

**Texture reference**:
A strong reference with the same lifecycle as a shader reference. `replace()`
supplies complete pixels/dimensions for a new renderer backing; it does not
resample or mutate the existing renderer texture.

**Sampler reference**:
A strong reference to an immutable sampler asset created from filtering and
addressing settings. Its lifecycle is independent of texture references;
materials pair their renderer handles.

**Asset update**:
The engine-thread operation that collects creation results, publishes successful
replacements, and requests retirement of superseded or unreferenced resources.
CPU asset references and renderer handles have separate lifetimes.

**Shader resource dependencies**:
The textures and samplers that a submitted frame might access, including
candidates selected indirectly by the GPU.

**Resource table index**:
A texture or sampler's logical location in its corresponding shader-visible
table. A CPU handle also carries a generation to distinguish successive resources
that occupy the same index.

**Shader parameter block**:
A named block of bytes supplied for a draw item, matching a compiled shader's
reflected parameter name and size.

**Shader interface**:
An owned snapshot of one device shader handle and its ordinary parameter block
names and sizes. It does not retain or follow replacement shaders.

**Material**:
A CPU value combining a shader interface, reusable named blocks, and declared
resource dependencies. Its blocks have distinct ownership from draw blocks;
its GPU handles are non-owning.

**Material draw**:
A frontend description of primitives and draw-specific parameters/dependencies,
with a non-owning material reference. The caller keeps the material alive through
every request build using the description.

**Render request builder**:
Collects owned draw descriptions and view settings. At build time it reads
borrowed materials, validates/combines parameter blocks and dependencies, and
produces an owned render request with no material references.
