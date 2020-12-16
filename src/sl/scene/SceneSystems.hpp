#pragma once

#include "sl/graphics/Renderer.h"

#include "systems/LightSystem.h"
#include "systems/ParticleEffectSystem.h"
#include "systems/RendererSystem.h"
#include "systems/ShadowSystem.h"
#include "systems/SkyboxSystem.h"

namespace sl::scene {

struct SceneSystems {
    SceneSystems(std::shared_ptr<graphics::Renderer> renderer)
        : rendererSystem(renderer)
        , pfxSystem(renderer)
        , skyboxSystem(renderer)
        , shadowSystem(renderer) {}

    systems::RendererSystem rendererSystem;
    systems::LightSystem lightSystem;
    systems::ParticleEffectSystem pfxSystem;
    systems::SkyboxSystem skyboxSystem;
    systems::ShadowSystem shadowSystem;
};
}
