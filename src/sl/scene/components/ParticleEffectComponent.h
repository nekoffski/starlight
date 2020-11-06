#pragma once

#include "sl/ecs/Component.h"

#include <memory>

#include "sl/physics/pfx/Particle.h"
#include "sl/physics/pfx/ParticleGeneratorSettings.h"

namespace sl::scene::components {

struct ParticleEffectComponent : public ecs::Component {
    explicit ParticleEffectComponent(math::Vec3 position = math::VEC_ZEROS, int maxParticles = 2500)
        : position(position)
        , maxParticles(maxParticles) {
    }

    math::Vec3 position;
    int maxParticles;
    physics::pfx::ParticleGeneratorSettings pfxGeneratorSettings;
    std::vector<physics::pfx::Particle> particles;
};
}
