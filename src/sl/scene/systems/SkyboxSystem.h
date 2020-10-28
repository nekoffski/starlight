#pragma once

#include <memory>

#include "sl/platform/fwd.h"
#include "sl/rendering/fwd.h"

namespace sl::scene::systems {

class SkyboxSystem {
public:
    explicit SkyboxSystem(std::shared_ptr<rendering::Renderer> renderer);
    void render(std::shared_ptr<platform::texture::Cubemap> cubemap, std::shared_ptr<platform::shader::Shader> cubemapShader,
        std::shared_ptr<rendering::camera::Camera> camera);

private:
    std::shared_ptr<platform::gpu::VertexArray> m_cubemapVertexArray;
    std::shared_ptr<rendering::Renderer> m_renderer;
};
}
