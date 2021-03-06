#pragma once

#include <memory>

#include "sl/async/AsyncEngine.hpp"
#include "sl/ecs/ComponentView.hpp"
#include "sl/gfx/fwd.h"
#include "sl/physx/pfx/ParticleGenerator.h"
#include "sl/scene/components/ParticleEffectComponent.h"
#include "sl/scene/components/TransformComponent.h"

namespace sl::physx::pfx {

using namespace scene;

class ParticleEffectsEngine {
public:
    explicit ParticleEffectsEngine();

    void update(ecs::ComponentView<components::ParticleEffectComponent>& pfxs, float deltaTime, gfx::camera::Camera&);

private:
    void updateParticleEffect(components::ParticleEffectComponent& pfx, float deltaTime, gfx::camera::Camera&);
    void updateParticle(physx::pfx::Particle& particle, float deltaTime);

    std::shared_ptr<async::Timer> m_pfxTimer;

    physx::pfx::ParticleGenerator m_particleGenerator;
};
}
