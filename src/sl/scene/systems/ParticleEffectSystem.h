#pragma once

#include <memory>

#include "sl/async/AsyncEngine.hpp"
#include "sl/physics/pfx/ParticleGenerator.h"
#include "sl/physics/pfx/ParticlesFuzzyController.h"
#include "sl/platform/fwd.h"
#include "sl/rendering/fwd.h"
#include "sl/scene/components/ParticleEffectComponent.h"

namespace sl::scene::systems {

class ParticleEffectSystem {
public:
    explicit ParticleEffectSystem(std::shared_ptr<rendering::Renderer> renderer);

    void renderParticleEffects(std::vector<components::ParticleEffectComponent>& pfxs, std::shared_ptr<rendering::camera::Camera> camera);
    void update(std::vector<components::ParticleEffectComponent>& pfxs, float deltaTime, std::shared_ptr<rendering::camera::Camera>);

private:
    void updateParticleEffect(components::ParticleEffectComponent& pfx, float deltaTime, std::shared_ptr<rendering::camera::Camera>);
    void updateParticle(physics::pfx::Particle& particle, float deltaTime);

    void beginParticleEffect();
    void renderParticle();
    void endParticleEffect();

    std::shared_ptr<platform::gpu::VertexArray> m_vao;
    std::shared_ptr<async::timer::Timer> m_pfxTimer;

    physics::pfx::ParticleGenerator m_particleGenerator;
    physics::pfx::ParticleFuzzyController m_particlesFuzzyController;
    std::shared_ptr<platform::shader::Shader> m_shader;
    std::shared_ptr<rendering::Renderer> m_renderer;
};
}
