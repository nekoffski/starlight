#include "ShadowRenderer.h"

#include "sl/gfx/LowLevelRenderer.h"
#include "sl/gfx/Shader.h"
#include "sl/gfx/ViewFrustum.h"
#include "sl/gfx/buffer/FrameBuffer.h"
#include "sl/utils/Globals.h"

namespace sl::gfx::renderer {

ShadowRenderer::ShadowRenderer(std::shared_ptr<gfx::LowLevelRenderer> renderer)
    : m_renderer(renderer)
    , m_shadowMapFrameBuffer(gfx::buffer::FrameBuffer::factory->create())
    , m_depthShader(gfx::Shader::load(
          GLOBALS().config.paths.shaders + "/depth_capture.vert", GLOBALS().config.paths.shaders + "/depth_capture.frag")) {
}

void ShadowRenderer::beginDepthCapture() {
    m_renderer->setTemporaryViewport(gfx::ViewFrustum::Viewport { 1024u, 1024u });
    m_shadowMapFrameBuffer->bind();
    m_renderer->clearBuffers(STARL_DEPTH_BUFFER_BIT);
    m_depthShader->enable();

    auto settings = m_renderer->getSettings();
    settings.cullFace = STARL_FRONT;
    m_renderer->setTemporarySettings(settings);
}

void ShadowRenderer::endDepthCapture() {
    m_shadowMapFrameBuffer->unbind();
    m_depthShader->disable();
    m_renderer->clearBuffers(STARL_DEPTH_BUFFER_BIT | STARL_COLOR_BUFFER_BIT);

    m_renderer->restoreSettings();
    m_renderer->restoreViewport();
}

void ShadowRenderer::setShadowMap(std::shared_ptr<sl::gfx::Texture> shadowMap) {
    m_shadowMapFrameBuffer->bindTexture(shadowMap);
}

std::shared_ptr<gfx::Shader> ShadowRenderer::getDepthShader() {
    return m_depthShader;
}
}
