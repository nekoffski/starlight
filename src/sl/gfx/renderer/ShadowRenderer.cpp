#include "ShadowRenderer.h"

#include "sl/gfx/LowLevelRenderer.h"
#include "sl/gfx/Shader.h"
#include "sl/gfx/Texture.h"
#include "sl/gfx/ViewFrustum.h"
#include "sl/gfx/buffer/FrameBuffer.h"
#include "sl/utils/Globals.h"

namespace sl::gfx::renderer {

ShadowRenderer::ShadowRenderer(LowLevelRenderer& renderer)
    : m_renderer(renderer)
    , m_shadowMapFrameBuffer(gfx::buffer::FrameBuffer::factory->create())
    , m_depthShader(gfx::Shader::load(
          GLOBALS().config.paths.shaders + "/depth_capture.vert", GLOBALS().config.paths.shaders + "/depth_capture.frag")) {
}

void ShadowRenderer::beginDepthCapture() {
    m_renderer.setTemporaryViewport(gfx::ViewFrustum::Viewport { Texture::shadowMapSize, Texture::shadowMapSize });
    m_shadowMapFrameBuffer->bind();
    m_renderer.clearBuffers(STARL_DEPTH_BUFFER_BIT);
    m_depthShader->enable();

    auto settings = m_renderer.getSettings();
    settings.cullFace = STARL_FRONT;
    m_renderer.setTemporarySettings(settings);
}

void ShadowRenderer::endDepthCapture() {
    m_shadowMapFrameBuffer->unbind();
    m_depthShader->disable();
    m_renderer.clearBuffers(STARL_DEPTH_BUFFER_BIT | STARL_COLOR_BUFFER_BIT);

    m_renderer.restoreSettings();
    m_renderer.restoreViewport();
}

void ShadowRenderer::bindShadowMap(sl::gfx::Texture& shadowMap) {
    m_shadowMapFrameBuffer->bindTexture(shadowMap);
}

gfx::Shader* ShadowRenderer::getDepthShader() {
    return m_depthShader.get();
}
}
