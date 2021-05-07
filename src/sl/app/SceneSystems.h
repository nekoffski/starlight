#pragma once

#include "sl/physx/PhysicsEngine.h"
#include "sl/physx/pfx/ParticleEffectsEngine.h"

namespace sl::app {

struct SceneSystems {
    explicit SceneSystems() = default;

    physx::pfx::ParticleEffectsEngine pfxEngine;
    physx::PhysicsEngine physxEngine;
};

}