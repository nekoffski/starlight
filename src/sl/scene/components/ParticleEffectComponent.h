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

    float minX = 0.0f;
    float maxX = 0.0f;
    float minY = 0.0f;
    float maxY = 0.0f;

    std::shared_ptr<gfx::Texture> texture = nullptr;
};
}
