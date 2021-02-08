#pragma once

#include <memory>

#include "sl/async/AsyncEngine.hpp"
#include "sl/ecs/ComponentView.hpp"
#include "sl/graphics/fwd.h"
#include "sl/physics/pfx/ParticleGenerator.h"
#include "sl/platform/fwd.h"
#include "sl/scene/components/ParticleEffectComponent.h"
#include "sl/scene/components/TransformComponent.h"

namespace sl::scene::systems {

class ParticleEffectSystem {
public:
    explicit ParticleEffectSystem();

    void update(ecs::ComponentView<components::ParticleEffectComponent>& pfxs, float deltaTime, std::shared_ptr<graphics::camera::Camera>);

private:
    void updateParticleEffect(components::ParticleEffectComponent& pfx, float deltaTime, std::shared_ptr<graphics::camera::Camera>);
    void updateParticle(physics::pfx::Particle& particle, float deltaTime);

    std::shared_ptr<async::Timer> m_pfxTimer;

    physics::pfx::ParticleGenerator m_particleGenerator;
};
}
