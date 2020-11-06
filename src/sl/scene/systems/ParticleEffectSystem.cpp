#include "ParticleEffectSystem.h"

#include "sl/asset/AssetManager.hpp"
#include "sl/core/perf/Profiler.h"
#include "sl/core/task/CallScheduler.hpp"
#include "sl/geometry/Geometry.hpp"
#include "sl/platform/shader/Shader.h"
#include "sl/rendering/Renderer.h"
#include "sl/rendering/camera/Camera.h"

namespace sl::scene::systems {

constexpr float PARTICLE_CLEANER_PERIOD = 2.0f;
constexpr int MAX_PARTICLE_PER_ITERATION = 100;

static void cleanRetiredParticles(std::vector<physics::pfx::Particle>& particles) {
    std::erase_if(particles, [](auto& particle) -> bool { return particle.scale <= 0 || particle.position.y >= 7.5f; });
}

ParticleEffectSystem::ParticleEffectSystem(std::shared_ptr<rendering::Renderer> renderer)
    : m_renderer(renderer)
    , m_vao(geometry::Geometry::getSquareVAO()) {

    m_shader = asset::AssetManager::load<platform::shader::Shader>(
        "/particle.vert", "/particle.frag");
}

void ParticleEffectSystem::renderParticleEffects(std::vector<components::ParticleEffectComponent>& pfxs,
    std::shared_ptr<rendering::camera::Camera> camera) {
    PRF_PROFILE_FUNCTION();

    m_shader->enable();
    m_shader->setUniform("view", camera->getViewMatrix());
    m_shader->setUniform("viewPos", camera->getPosition());
    beginParticleEffect();

    for (auto& pfx : pfxs) {
        m_shader->setUniform("model", math::translate(pfx.position));

        for (auto& particle : pfx.particles) {
            m_shader->setUniform("localModel", math::translate(particle.position) * math::scale(particle.scale));
            m_shader->setUniform("color", particle.color);
            renderParticle();
        }
    }

    endParticleEffect();
    m_shader->disable();
}

void ParticleEffectSystem::update(std::vector<components::ParticleEffectComponent>& pfxs, float deltaTime) {
    for (auto& pfx : pfxs)
        updateParticleEffect(pfx, deltaTime);
}

void ParticleEffectSystem::updateParticleEffect(components::ParticleEffectComponent& pfx, float deltaTime) {
    auto& particles = pfx.particles;
    for (auto& particle : particles)
        updateParticle(particle, deltaTime);

    core::task::CallScheduler::callIfExpired(
        deltaTime * PARTICLE_CLEANER_PERIOD, "pfxCleaner", cleanRetiredParticles, particles);

    if (int delta = pfx.maxParticles - particles.size(); delta > 0) {
        auto n = std::min(delta, MAX_PARTICLE_PER_ITERATION);
        auto generatedParticles = m_particleGenerator.generate(pfx.pfxGeneratorSettings, n);

        particles.reserve(n);
        particles.insert(particles.end(), generatedParticles.begin(), generatedParticles.end());
    }
}

void ParticleEffectSystem::updateParticle(physics::pfx::Particle& particle, float deltaTime) {
    auto& pos = particle.position;
    pos += deltaTime * particle.speed * particle.direction;
    particle.direction = math::normalize(particle.direction +
        particle.deltaDirection * particle.directionFactor);
    particle.scale -= deltaTime * particle.deltaScale;
}

void ParticleEffectSystem::beginParticleEffect() {
    m_shader->setUniform("projection", m_renderer->getProjectionMatrix());
    m_vao->bind();
}

void ParticleEffectSystem::renderParticle() {
    m_renderer->renderVertexArray(m_vao);
}

void ParticleEffectSystem::endParticleEffect() {
    m_vao->unbind();
}
}
