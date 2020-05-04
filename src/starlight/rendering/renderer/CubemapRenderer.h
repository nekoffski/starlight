#pragma once

#include <memory>

#include <starlight/rendering/renderer/Renderer.h>

namespace starl::platform {
namespace shader {
    class Shader;
}

namespace gpu {
    class VertexArray;
}

namespace texture {
    class Cubemap;
}
}

namespace starl::rendering::renderer {

class CubemapRenderer : public Renderer {
public:
    explicit CubemapRenderer(framework::graphics::LowLevelRenderer&);

    void render(const std::shared_ptr<platform::texture::Cubemap>& cubemap,
        const std::shared_ptr<platform::shader::Shader>& cubemapShader,
        const std::shared_ptr<framework::graphics::camera::Camera>& camera);

private:
    std::shared_ptr<platform::gpu::VertexArray> m_cubemapVertexArray;
};
}