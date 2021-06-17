#pragma once

#include <memory>

#include "sl/gfx/LowLevelRenderer.h"
#include "sl/gfx/Shader.h"
#include "sl/gfx/buffer/FrameBuffer.h"

namespace sl::gfx::renderer {

class ShadowRenderer {
public:
    explicit ShadowRenderer(LowLevelRenderer& renderer);

    void beginDepthCapture();
    void endDepthCapture();

    // void bindShadowMap(sl::gfx::Texture&);

    gfx::buffer::FrameBuffer* getShadowMapFrameBuffer();
    gfx::Shader* getDepthShader();

private:
    LowLevelRenderer& m_renderer;
    std::shared_ptr<gfx::buffer::FrameBuffer> m_shadowMapFrameBuffer;
    std::shared_ptr<gfx::Shader> m_depthShader;
};
}
