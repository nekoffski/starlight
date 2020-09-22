#pragma once

#include "Renderer.h"

#include <memory>

namespace sl::platform {
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

namespace sl::rendering::renderer {

class CubemapRenderer : public Renderer {
public:
    explicit CubemapRenderer(lowlevel::LowLevelRenderer&);

    void render(const std::shared_ptr<platform::texture::Cubemap>& cubemap,
        const std::shared_ptr<platform::shader::Shader>& cubemapShader,
        const std::shared_ptr<rendering::camera::Camera>& camera);

private:
    std::shared_ptr<platform::gpu::VertexArray> m_cubemapVertexArray;
};
}