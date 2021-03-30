#include "ParticleEffectSystem.h"

#include "sl/asset/AssetLoader.hpp"
#include "sl/async/AsyncEngine.hpp"
#include "sl/core/Profiler.h"
#include "sl/ecs/ComponentView.hpp"
#include "sl/gfx/Shader.h"
#include "sl/gfx/camera/Camera.h"
#include "sl/scene/components/TransformComponent.h"
#include "sl/utils/Globals.h"

namespace sl::scene::systems {

constexpr float particleCleanerSleepTime = 0.1f;
constexpr int MAX_PARTICLE_PER_ITERATION = 1000;

static void cleanRetiredParticles(std::vector<physx::pfx::Particle>& particles) {
    std::erase_if(particles, [](auto& particle) -> bool { return particle.scale <= 0 || particle.position.y >= 7.5f; });
}

ParticleEffectSystem::ParticleEffectSystem() {
    m_pfxTimer = async::AsyncEngine::createTimer(particleCleanerSleepTime);
}

void ParticleEffectSystem::update(ecs::ComponentView<components::ParticleEffectComponent>& pfxs, float deltaTime,
    std::shared_ptr<gfx::camera::Camera> camera) {
    for (auto& pfx : pfxs)
        updateParticleEffect(pfx, deltaTime, camera);
}

void ParticleEffectSystem::updateParticleEffect(components::ParticleEffectComponent& pfx, float deltaTime, std::shared_ptr<gfx::camera::Camera> camera) {
    pfx.maxParticles = MAX_PARTICLE_PER_ITERATION; 

    auto& particles = pfx.particles;
    for (auto& particle : particles)
        updateParticle(particle, deltaTime);

    if (not m_pfxTimer->asyncSleep())
        std::erase_if(particles, [](auto& particle) -> bool { return particle.scale <= 0 || particle.position.y >= 7.5f; });

    int maxParticlesPerIteration = std::max(1, pfx.maxParticles / 100);
    if (int delta = pfx.maxParticles - particles.size(); delta > 0) {
        auto n = std::min(delta, maxParticlesPerIteration);
        auto generatedParticles = m_particleGenerator.generate(pfx.pfxGeneratorSettings, n);

        particles.reserve(n);
        particles.insert(particles.end(), generatedParticles.begin(), generatedParticles.end());
    }
}

void ParticleEffectSystem::updateParticle(physx::pfx::Particle& particle, float deltaTime) {
    auto& pos = particle.position;
    pos += deltaTime * particle.speed * particle.direction;
    particle.direction = math::normalize(particle.direction +
        particle.deltaDirection * particle.directionFactor);
    particle.scale -= deltaTime * particle.deltaScale;
}
}
