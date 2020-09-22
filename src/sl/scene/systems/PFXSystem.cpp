#include "PFXSystem.h"

#include "sl/asset/AssetManager.hpp"
#include "sl/core/perf/Profiler.h"
#include "sl/core/task/CallScheduler.hpp"
#include "sl/platform/shader/Shader.h"
#include "sl/rendering/RendererProxy.h"
#include "sl/rendering/camera/Camera.h"

namespace sl::scene::systems {

constexpr float PARTICLE_CLEANER_PERIOD = 2.0f;
constexpr int MAX_PARTICLE_PER_ITERATION = 100;

static void cleanRetiredParticles(std::vector<rendering::pfx::Particle>& particles) {
    std::erase_if(particles, [](auto& particle) -> bool { return particle.scale <= 0 || particle.position.y >= 7.5f; });
}

PFXSystem::PFXSystem(std::shared_ptr<rendering::RendererProxy> renderer)
    : m_renderer(renderer) {
    m_shader = asset::AssetManager::load<platform::shader::Shader>(
        "/particle.vert", "/particle.frag");
}

void PFXSystem::renderPFXs(std::vector<components::PFXComponent>& pfxs, std::shared_ptr<rendering::camera::Camera> camera) {
    PRF_PROFILE_FUNCTION();

    for (auto& pfx : pfxs) {
        m_shader->enable();
        m_shader->setUniform("view", camera->getViewMatrix());
        m_shader->setUniform("viewPos", camera->getPosition());
        m_shader->setUniform("model", math::translate(pfx.position));

        for (auto& particle : pfx.particles) {
            m_shader->setUniform("localModel", math::translate(particle.position) * math::scale(particle.scale));
            m_shader->setUniform("color", particle.color);
            m_renderer->beginPFX(m_shader);
        }

        // m_renderer->endPFX();
        m_shader->disable();
    }
}

void PFXSystem::update(std::vector<components::PFXComponent>& pfxs, float deltaTime) {
    for (auto& pfx : pfxs)
        updateParticleEffect(pfx, deltaTime);
}

void PFXSystem::updateParticleEffect(components::PFXComponent& pfx, float deltaTime) {
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

void PFXSystem::updateParticle(rendering::pfx::Particle& particle, float deltaTime) {
    auto& pos = particle.position;
    pos += deltaTime * particle.speed * particle.direction;
    particle.direction = math::normalize(particle.direction +
        particle.deltaDirection * particle.directionFactor);
    particle.scale -= deltaTime * particle.deltaScale;
}
}
