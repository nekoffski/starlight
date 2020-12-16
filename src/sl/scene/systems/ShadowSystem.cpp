#include "ShadowSystem.h"

#include "sl/asset/AssetManager.hpp"
#include "sl/platform/gpu/FrameBuffer.h"
#include "sl/platform/shader/Shader.h"
#include "sl/graphics/Renderer.h"

namespace sl::scene::systems {

ShadowSystem::ShadowSystem(std::shared_ptr<graphics::Renderer> renderer)
    : m_renderer(renderer)
    , m_shadowMapFrameBuffer(platform::gpu::FrameBuffer::create())
    , m_depthShader(asset::AssetManager::loadLocalPath<platform::shader::Shader>("/depth_capture.vert", "/depth_capture.frag")) {
}

void ShadowSystem::beginDepthCapture() {
    m_renderer->setTemporaryViewport(1024u, 1024u);
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

void ShadowSystem::setShadowMap(std::shared_ptr<platform::texture::Texture> shadowMap) {
    m_shadowMapFrameBuffer->bindTexture(shadowMap);
}

std::shared_ptr<platform::shader::Shader> ShadowSystem::getDepthShader() {
    return m_depthShader;
}
}
