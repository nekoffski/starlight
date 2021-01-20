#pragma once

#include "systems/ParticleEffectSystem.h"

namespace sl::scene {

struct SceneSystems {
    SceneSystems()
        : pfxSystem() {}

    systems::ParticleEffectSystem pfxSystem;
};
}
