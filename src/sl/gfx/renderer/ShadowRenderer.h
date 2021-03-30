#pragma once

#include <memory>

#include "sl/gfx/Shader.h"
#include "sl/gfx/buffer/FrameBuffer.h"
#include "sl/gfx/fwd.h"
#include "sl/platform/fwd.h"

namespace sl::gfx::renderer {

class ShadowRenderer {
public:
    explicit ShadowRenderer(std::shared_ptr<gfx::LowLevelRenderer> renderer);

    void beginDepthCapture();
    void endDepthCapture();

    void setShadowMap(std::shared_ptr<sl::gfx::Texture>);

    std::shared_ptr<gfx::Shader> getDepthShader();

private:
    std::shared_ptr<gfx::LowLevelRenderer> m_renderer;
    std::shared_ptr<gfx::buffer::FrameBuffer> m_shadowMapFrameBuffer;
    std::shared_ptr<gfx::Shader> m_depthShader;
};
}
