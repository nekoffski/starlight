#pragma once

#include <memory>

#include "sl/platform/fwd.h"
#include "sl/graphics/fwd.h"
#include "sl/graphics/buffer/VertexArray.h"

namespace sl::scene::systems {

class SkyboxSystem {
public:
    explicit SkyboxSystem(std::shared_ptr<graphics::Renderer> renderer);
    void render(std::shared_ptr<sl::graphics::Cubemap> cubemap, std::shared_ptr<graphics::Shader> cubemapShader,
        std::shared_ptr<graphics::camera::Camera> camera);

private:
    std::shared_ptr<graphics::buffer::VertexArray> m_cubemapVertexArray;
    std::shared_ptr<graphics::Renderer> m_renderer;
};
}
