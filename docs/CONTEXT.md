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
The current asset wrapper tracking asynchronous creation of a device shader
and exposing its handle and creation state.

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
