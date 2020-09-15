#pragma once

#include "starlight/asset/AssetManager.hpp"
#include "starlight/platform/shader/Shader.h"
#include "starlight/rendering/RendererProxy.h"
#include "starlight/rendering/camera/Camera.h"
#include "starlight/rendering/pfx/ParticleGenerator.h"
#include "starlight/scene/components/PFXComponent.h"

namespace sl::scene::systems {

class PFXSystem {
public:
    explicit PFXSystem(std::shared_ptr<rendering::RendererProxy> renderer)
        : m_renderer(renderer) {
        m_shader = asset::AssetManager::load<platform::shader::Shader>(
            "/particle.vert", "/particle.frag");
    }

    void renderPFXs(std::vector<components::PFXComponent>& pfxs, std::shared_ptr<rendering::camera::Camera> camera) {
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

    void update(std::vector<components::PFXComponent>& pfxs, float deltaTime) {
        for (auto& pfx : pfxs)
            updateParticleEffect(pfx, deltaTime);
    }

private:
    void updateParticleEffect(components::PFXComponent& pfx, float deltaTime) {
        auto& particles = pfx.particles;
        for (auto& particle : particles)
            updateParticle(particle, deltaTime);

        // TODO: corner case of particle should be configurable
        std::erase_if(particles, [](auto& particle) -> bool { return particle.scale <= 0; });

        if (int n = pfx.maxParticles - particles.size(); n > 0) {
            auto generatedParticles = m_particleGenerator.generate(pfx.pfxGeneratorSettings, n);

            particles.reserve(n);
            particles.insert(particles.end(), generatedParticles.begin(), generatedParticles.end());
        }
    }

    void updateParticle(rendering::pfx::Particle& particle, float deltaTime) {
        auto& pos = particle.position;
        pos += deltaTime * particle.speed * particle.direction;
        particle.scale -= deltaTime * 0.1f;
    }

    rendering::pfx::ParticleGenerator m_particleGenerator;
    std::shared_ptr<platform::shader::Shader> m_shader;
    std::shared_ptr<rendering::RendererProxy> m_renderer;
};
}
