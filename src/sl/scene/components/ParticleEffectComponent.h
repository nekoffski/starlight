#pragma once

#include "sl/ecs/Component.h"

#include <memory>

#include "sl/physx/pfx/Particle.h"
#include "sl/physx/pfx/ParticleGeneratorSettings.h"

namespace sl::scene::components {

struct ParticleEffectComponent : ecs::Component {
    explicit ParticleEffectComponent(math::Vec3 position = math::VEC_ZEROS, int maxParticles = 2500)
        : position(position)
        , maxParticles(maxParticles) {
        name = "ParticleEffectComponent";
    }

    math::Vec3 position;
    int maxParticles;
    physx::pfx::ParticleGeneratorSettings pfxGeneratorSettings;
    std::vector<physx::pfx::Particle> particles;
};
}
