#include "ParticleEffectRenderer.h"

#include <kc/core/Profiler.h>
#include "sl/ecs/ComponentView.hpp"
#include "sl/gfx/Renderer.h"
#include "sl/gfx/Shader.h"
#include "sl/gfx/camera/Camera.h"
#include "sl/scene/components/TransformComponent.h"
#include "sl/utils/Globals.h"

namespace sl::gfx::renderer {

ParticleEffectRenderer::ParticleEffectRenderer(Renderer& renderer)
    : m_renderer(renderer)
    , m_vao(utils::Globals::get()->geom->frontSquareVAO)
    , m_shader(utils::Globals::get()->shaders->pfxShader) {
}

void ParticleEffectRenderer::renderParticleEffects(ecs::ComponentView<scene::components::ParticleEffectComponent> pfxs,
    ecs::ComponentView<scene::components::TransformComponent> transforms, gfx::camera::Camera& camera) {
    PROFILE_FUNCTION();

    m_shader->enable();
    m_shader->setUniform("view", camera.getViewMatrix());

    beginParticleEffect(camera);

    for (auto& pfx : pfxs) {
        m_shader->setUniform("minX", pfx.minX);
        m_shader->setUniform("minY", pfx.minY);
        m_shader->setUniform("maxX", pfx.maxX);
        m_shader->setUniform("maxY", pfx.maxY);

        auto& transform = transforms.getByEntityId(pfx.ownerEntityId);
        m_shader->setUniform("model", math::translate(pfx.position) * transform.transformation);

        bool hasTexture = pfx.texture.hasValue();

        if (hasTexture)
            pfx.texture->bind();

        m_shader->setUniform("hasTexture", hasTexture);

        for (auto& particle : pfx.particles) {
            m_shader->setUniform("scale", particle.scale);
            m_shader->setUniform("localModel", math::translate(particle.position));
            m_shader->setUniform("position", particle.position);
            m_shader->setUniform("color", particle.color);
            renderParticle();
        }

        if (hasTexture)
            pfx.texture->unbind();
    }

    endParticleEffect();
    m_shader->disable();
}

void ParticleEffectRenderer::beginParticleEffect(gfx::camera::Camera& camera) {
    m_shader->setUniform("projection", camera.getProjectionMatrix());
    m_vao->bind();
}

void ParticleEffectRenderer::renderParticle() {
    m_renderer.renderVertexArray(*m_vao);
}

void ParticleEffectRenderer::endParticleEffect() {
    m_vao->unbind();
}
}
