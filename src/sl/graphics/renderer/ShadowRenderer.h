#pragma once

#include <memory>

#include "sl/graphics/Shader.h"
#include "sl/graphics/buffer/FrameBuffer.h"
#include "sl/graphics/fwd.h"
#include "sl/platform/fwd.h"

namespace sl::graphics::renderer {

class ShadowRenderer {
public:
    explicit ShadowRenderer(std::shared_ptr<graphics::LowLevelRenderer> renderer);

    void beginDepthCapture();
    void endDepthCapture();

    void setShadowMap(std::shared_ptr<sl::graphics::Texture>);

    std::shared_ptr<graphics::Shader> getDepthShader();

private:
    std::shared_ptr<graphics::LowLevelRenderer> m_renderer;
    std::shared_ptr<graphics::buffer::FrameBuffer> m_shadowMapFrameBuffer;
    std::shared_ptr<graphics::Shader> m_depthShader;
};
}
