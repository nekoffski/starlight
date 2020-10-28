#pragma once

#include "sl/platform/fwd.h"
#include "sl/rendering/fwd.h"
#include "sl/rendering/pfx/ParticleGenerator.h"
#include "sl/scene/components/ParticleEffectComponent.h"

namespace sl::scene::systems {

class ParticleEffectSystem {
public:
    explicit ParticleEffectSystem(std::shared_ptr<rendering::Renderer> renderer);

    void renderParticleEffects(std::vector<components::ParticleEffectComponent>& pfxs, std::shared_ptr<rendering::camera::Camera> camera);
    void update(std::vector<components::ParticleEffectComponent>& pfxs, float deltaTime);

private:
    void updateParticleEffect(components::ParticleEffectComponent& pfx, float deltaTime);
    void updateParticle(rendering::pfx::Particle& particle, float deltaTime);

    void beginParticleEffect();
    void renderParticle();
    void endParticleEffect();

    std::shared_ptr<platform::gpu::VertexArray> m_vao;

    rendering::pfx::ParticleGenerator m_particleGenerator;
    std::shared_ptr<platform::shader::Shader> m_shader;
    std::shared_ptr<rendering::Renderer> m_renderer;
};
}
