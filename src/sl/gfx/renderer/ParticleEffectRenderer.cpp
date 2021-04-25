#include "ParticleEffectRenderer.h"

#include "sl/async/AsyncEngine.hpp"
#include "sl/core/Profiler.h"
#include "sl/ecs/ComponentView.hpp"
#include "sl/gfx/LowLevelRenderer.h"
#include "sl/gfx/Shader.h"
#include "sl/gfx/camera/Camera.h"
#include "sl/scene/components/TransformComponent.h"
#include "sl/utils/Globals.h"

namespace sl::gfx::renderer {

ParticleEffectRenderer::ParticleEffectRenderer(std::shared_ptr<gfx::LowLevelRenderer> renderer)
    : m_renderer(renderer)
    , m_vao(GLOBALS().geom->squareVAO) {

    m_shader = gfx::Shader::load(
        GLOBALS().config.paths.shaders + "/particle.vert", GLOBALS().config.paths.shaders + "/particle.frag");
}

void ParticleEffectRenderer::renderParticleEffects(ecs::ComponentView<scene::components::ParticleEffectComponent> pfxs,
    ecs::ComponentView<scene::components::TransformComponent> transforms, std::shared_ptr<gfx::camera::Camera> camera) {
    SL_PROFILE_FUNCTION();

    m_shader->enable();
    m_shader->setUniform("view", camera->getViewMatrix());
    m_shader->setUniform("viewPos", camera->getPosition());
    beginParticleEffect(camera);

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

void ParticleEffectRenderer::beginParticleEffect(std::shared_ptr<gfx::camera::Camera> camera) {
    m_shader->setUniform("projection", camera->getProjectionMatrix());
    m_vao->bind();
}

void ParticleEffectRenderer::renderParticle() {
    m_renderer->renderVertexArray(m_vao);
}

void ParticleEffectRenderer::endParticleEffect() {
    m_vao->unbind();
}
}
