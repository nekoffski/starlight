#pragma once

#include <memory>

#include "sl/platform/fwd.h"
#include "sl/platform/shader/Shader.h"
#include "sl/graphics/fwd.h"

namespace sl::scene::systems {

class ShadowSystem {
public:
    explicit ShadowSystem(std::shared_ptr<graphics::Renderer> renderer);

    void beginDepthCapture();
    void endDepthCapture();

    void setShadowMap(std::shared_ptr<platform::texture::Texture>);

    std::shared_ptr<platform::shader::Shader> getDepthShader();

private:
    std::shared_ptr<graphics::Renderer> m_renderer;
    std::shared_ptr<platform::gpu::FrameBuffer> m_shadowMapFrameBuffer;
    std::shared_ptr<platform::shader::Shader> m_depthShader;
};
}
