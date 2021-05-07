#pragma once

#include "sl/physx/pfx/ParticleEffectsEngine.h"

namespace sl::app {

struct SceneSystems {
    SceneSystems()
        : pfxEngine() { }

    physx::pfx::ParticleEffectsEngine pfxEngine;
};

}