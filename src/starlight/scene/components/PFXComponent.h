#pragma once

#include "starlight/ecs/Component.h"

#include <memory>

#include "starlight/rendering/pfx/Particle.h"
#include "starlight/rendering/pfx/ParticleGeneratorSettings.h"

namespace sl::scene::components {

struct PFXComponent : public ecs::Component {
    explicit PFXComponent(math::Vec3 position = math::VEC_ZEROS, int maxParticles = 2500)
        : position(position)
        , maxParticles(maxParticles) {
    }

    math::Vec3 position;
    int maxParticles;
    rendering::pfx::ParticleGeneratorSettings pfxGeneratorSettings;
    std::vector<rendering::pfx::Particle> particles;
};
}
