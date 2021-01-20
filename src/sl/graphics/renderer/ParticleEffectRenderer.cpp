#include "ParticleEffectRenderer.h"

#include "sl/asset/AssetManager.hpp"
#include "sl/async/AsyncEngine.hpp"
#include "sl/core/Profiler.h"
#include "sl/ecs/ComponentView.hpp"
#include "sl/graphics/LowLevelRenderer.h"
#include "sl/graphics/Shader.h"
#include "sl/graphics/camera/Camera.h"
#include "sl/scene/components/TransformComponent.h"
#include "sl/utils/Globals.h"

namespace sl::graphics::renderer {

ParticleEffectRenderer::ParticleEffectRenderer(std::shared_ptr<graphics::LowLevelRenderer> renderer)
    : m_renderer(renderer)
    , m_vao(utils::Globals::geometry->squareVAO) {

    m_shader = asset::AssetManager::loadLocalPath<graphics::Shader>(
        "/particle.vert", "/particle.frag");
}

void ParticleEffectRenderer::renderParticleEffects(ecs::ComponentView<scene::components::ParticleEffectComponent> pfxs,
    ecs::ComponentView<scene::components::TransformComponent> transforms, std::shared_ptr<graphics::camera::Camera> camera) {
    SL_PROFILE_FUNCTION();

    m_shader->enable();
    m_shader->setUniform("view", camera->getViewMatrix());
    m_shader->setUniform("viewPos", camera->getPosition());
    beginParticleEffect();

    for (auto& pfx : pfxs) {
        auto& transform = transforms.getByEntityId(pfx.ownerEntityId);
        m_shader->setUniform("model", math::translate(pfx.position) * transform());

        for (auto& particle : pfx.particles) {
            m_shader->setUniform("localModel", math::translate(particle.position) * math::scale(particle.scale));
            m_shader->setUniform("color", particle.color);
            renderParticle();
        }
    }

    endParticleEffect();
    m_shader->disable();
}

void ParticleEffectRenderer::beginParticleEffect() {
    m_shader->setUniform("projection", m_renderer->getProjectionMatrix());
    m_vao->bind();
}

void ParticleEffectRenderer::renderParticle() {
    m_renderer->renderVertexArray(m_vao);
}

void ParticleEffectRenderer::endParticleEffect() {
    m_vao->unbind();
}
}
