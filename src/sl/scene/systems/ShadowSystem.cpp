#include "ShadowSystem.h"

#include "sl/asset/AssetManager.hpp"
#include "sl/graphics/Renderer.h"
#include "sl/graphics/Shader.h"
#include "sl/graphics/Viewport.h"
#include "sl/graphics/buffer/FrameBuffer.h"
#include "sl/platform/gpu/fwd.h"

namespace sl::scene::systems {

ShadowSystem::ShadowSystem(std::shared_ptr<graphics::Renderer> renderer)
    : m_renderer(renderer)
    , m_shadowMapFrameBuffer(graphics::buffer::FrameBuffer::factory->create())
    , m_depthShader(asset::AssetManager::loadLocalPath<graphics::Shader>("/depth_capture.vert", "/depth_capture.frag")) {
}

void ShadowSystem::beginDepthCapture() {
    m_renderer->setTemporaryViewport(graphics::Viewport{ 1024u, 1024u });
    m_shadowMapFrameBuffer->bind();
    m_renderer->clearBuffers(STARL_DEPTH_BUFFER_BIT);
    m_depthShader->enable();
}

void ShadowSystem::endDepthCapture() {
    m_shadowMapFrameBuffer->unbind();
    m_depthShader->disable();
    m_renderer->clearBuffers(STARL_DEPTH_BUFFER_BIT | STARL_COLOR_BUFFER_BIT);
    m_renderer->restoreViewport();
}

void ShadowSystem::setShadowMap(std::shared_ptr<sl::graphics::Texture> shadowMap) {
    m_shadowMapFrameBuffer->bindTexture(shadowMap);
}

std::shared_ptr<graphics::Shader> ShadowSystem::getDepthShader() {
    return m_depthShader;
}
}
