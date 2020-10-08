#pragma once

#include <memory>

#include "sl/rendering/renderer/Renderer.h"

#include "sl/platform/fwd.h"
#include "sl/rendering/fwd.h"

namespace sl::rendering::renderer {

class ParticleRenderer : public Renderer {
public:
    explicit ParticleRenderer(lowlevel::LowLevelRenderer& lowLevelRenderer);

    void begin(std::shared_ptr<platform::shader::Shader> shader);
    void render();
    void end();

private:
    std::shared_ptr<platform::gpu::VertexArray> m_vao;
};
}