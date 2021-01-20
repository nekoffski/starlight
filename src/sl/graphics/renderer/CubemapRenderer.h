#pragma once

#include <memory>

#include "sl/graphics/buffer/VertexArray.h"
#include "sl/graphics/fwd.h"
#include "sl/platform/fwd.h"

namespace sl::graphics::renderer {

class CubemapRenderer {
public:
    explicit CubemapRenderer(std::shared_ptr<LowLevelRenderer> renderer);
    void render(std::shared_ptr<Cubemap> cubemap, std::shared_ptr<Shader> cubemapShader,
        std::shared_ptr<camera::Camera> camera);

private:
    std::shared_ptr<buffer::VertexArray> m_cubemapVertexArray;
    std::shared_ptr<LowLevelRenderer> m_renderer;
};
}
