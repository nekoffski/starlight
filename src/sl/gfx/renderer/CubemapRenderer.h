#pragma once

#include <memory>

#include "sl/gfx/Cubemap.h"
#include "sl/gfx/LowLevelRenderer.h"
#include "sl/gfx/buffer/VertexArray.h"

namespace sl::gfx::renderer {

class CubemapRenderer {
public:
    explicit CubemapRenderer(std::shared_ptr<LowLevelRenderer> renderer);
    void render(Cubemap& cubemap, Shader& cubemapShader, camera::Camera& camera);

private:
    std::shared_ptr<buffer::VertexArray> m_cubemapVertexArray;
    std::shared_ptr<LowLevelRenderer> m_renderer;
};
}
