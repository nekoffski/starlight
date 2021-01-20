#include "ShadowRenderer.h"

#include "sl/asset/AssetManager.hpp"
#include "sl/graphics/LowLevelRenderer.h"
#include "sl/graphics/Shader.h"
#include "sl/graphics/Viewport.h"
#include "sl/graphics/buffer/FrameBuffer.h"
#include "sl/platform/gpu/fwd.h"

namespace sl::graphics::renderer {

ShadowRenderer::ShadowRenderer(std::shared_ptr<graphics::LowLevelRenderer> renderer)
    : m_renderer(renderer)
    , m_shadowMapFrameBuffer(graphics::buffer::FrameBuffer::factory->create())
    , m_depthShader(asset::AssetManager::loadLocalPath<graphics::Shader>("/depth_capture.vert", "/depth_capture.frag")) {
}

void ShadowRenderer::beginDepthCapture() {
    m_renderer->setTemporaryViewport(graphics::Viewport{ 1024u, 1024u });
    m_shadowMapFrameBuffer->bind();
    m_renderer->clearBuffers(STARL_DEPTH_BUFFER_BIT);
    m_depthShader->enable();
}

void ShadowRenderer::endDepthCapture() {
    m_shadowMapFrameBuffer->unbind();
    m_depthShader->disable();
    m_renderer->clearBuffers(STARL_DEPTH_BUFFER_BIT | STARL_COLOR_BUFFER_BIT);
    m_renderer->restoreViewport();
}

void ShadowRenderer::setShadowMap(std::shared_ptr<sl::graphics::Texture> shadowMap) {
    m_shadowMapFrameBuffer->bindTexture(shadowMap);
}

std::shared_ptr<graphics::Shader> ShadowRenderer::getDepthShader() {
    return m_depthShader;
}
}
