# Implementation status

This node records current code, rather than future plans. For the documentation
policy (including no ADRs), see [Maintaining knowledge nodes](README.md#maintaining-knowledge-nodes).

- **Renderer thread:** [RenderingSystem](../src/engine/starlight/renderer/RenderingSystem.cpp)
  starts a [RenderExecutor](../src/engine/starlight/renderer/executor/RenderExecutor.cpp).
  [RendererProxy](../src/engine/starlight/renderer/RendererProxy.cpp) submits commands
  to it.
- **Slang shaders:** [compile_shaders.sh](../bin/compile_shaders.sh) invokes
  `slangc` and emits compiled shaders plus reflection JSON.
  [ShaderLoader](../src/engine/starlight/asset/loaders/ShaderLoader.cpp) reads the
  compiled bytes and metadata.
- **Metal:** [MetalDevice](../src/engine/starlight/platform/metal/MetalDevice.cpp)
  implements the render device selected by
  [Platform](../src/engine/starlight/platform/Platform.cpp) for Metal builds.
- **Sampled textures:** [RendererProxy](../src/engine/starlight/renderer/RendererProxy.cpp)
  creates immutable RGBA8 textures and separate samplers. Metal uses indexed
  resource tables plus explicit parameter blocks; the sandbox uploads a procedural
  checkerboard. See [Shader pipeline](shader-pipeline.md) for limits.
- **Transforms:** [Shader parameter blocks](../src/engine/starlight/renderer/core/ShaderDataLayout.hh)
  copy typed values into owned bytes. The sandbox updates a `Mat4f` per frame;
  [simple.slang](../assets/shaders/simple.slang) applies it to vertex positions.
- **Vulkan:** source files exist in
  [platform/vulkan](../src/engine/starlight/platform/vulkan/), but
  `Platform::createRenderDevice()` returns null in the Vulkan branch.

See [Renderer design](renderer-design.md) for the implemented frame path.
