#pragma once

#include "sl/platform/fwd.h"
#include "sl/rendering/fwd.h"
#include "sl/rendering/pfx/ParticleGenerator.h"
#include "sl/scene/components/PFXComponent.h"

namespace sl::scene::systems {

class PFXSystem {
public:
    explicit PFXSystem(std::shared_ptr<rendering::RendererProxy> renderer);

    void renderPFXs(std::vector<components::PFXComponent>& pfxs, std::shared_ptr<rendering::camera::Camera> camera);
    void update(std::vector<components::PFXComponent>& pfxs, float deltaTime);

private:
    void updateParticleEffect(components::PFXComponent& pfx, float deltaTime);
    void updateParticle(rendering::pfx::Particle& particle, float deltaTime);

    rendering::pfx::ParticleGenerator m_particleGenerator;
    std::shared_ptr<platform::shader::Shader> m_shader;
    std::shared_ptr<rendering::RendererProxy> m_renderer;
};
}
