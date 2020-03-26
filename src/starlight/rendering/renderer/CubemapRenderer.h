#pragma once

#include <memory>

#include <starlight/rendering/renderer/Renderer.h>

namespace starl::platform {
namespace shader{
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

    void render() override;

    void setCubemapShader(std::shared_ptr<platform::shader::Shader> shader) {
        m_cubemapShader = shader;
    }

    void setCubemap(std::shared_ptr<platform::texture::Cubemap> cubemap) {
        m_cubemap = cubemap;
    }

private:
    std::shared_ptr<platform::texture::Cubemap> m_cubemap;
    std::shared_ptr<platform::shader::Shader> m_cubemapShader;
    std::shared_ptr<platform::gpu::VertexArray> m_cubemapVertexArray;
};

}