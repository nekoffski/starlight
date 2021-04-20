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
    }

    void onGui(gui::GuiApi& gui) override {
        gui.pushId(ownerEntityId);

        if (beginComponentTreeNode(gui, "ParticleEffect")) {
            gui.displayText("Position");
            gui.dragFloat3(gui::createHiddenLabel("pfxPositon"), position, 0.1f);

            gui.displayText("Max particles");
            gui.dragInt(gui::createHiddenLabel("pfxMaxParticles"), maxParticles, 1.0, 1, 12500);

            auto& generatorSettings = pfxGeneratorSettings;

            gui.displayText("Radius");
            gui.dragFloat(gui::createHiddenLabel("pfxRadius"), generatorSettings.radius, 0.01f, 0.0f, 25.0f);

            gui.displayText("Min color");
            gui.dragFloat3(gui::createHiddenLabel("pfxMinColor"), generatorSettings.minColor, 0.01f, 0.0f, 1.0f);

            gui.displayText("Max color");
            gui.dragFloat3(gui::createHiddenLabel("pfxMaxColor"), generatorSettings.maxColor, 0.01f, 0.0f, 1.0f);

            gui.displayText("Min speed");
            gui.dragFloat(gui::createHiddenLabel("pfxMinSpeed"), generatorSettings.minSpeed, 0.005f, 0.0f, 15.0f);

            gui.displayText("Max speed");
            gui.dragFloat(gui::createHiddenLabel("pfxMaxSpeed"), generatorSettings.maxSpeed, 0.005f, 0.0f, 15.0f);

            gui.displayText("Min scale");
            gui.dragFloat(gui::createHiddenLabel("pfxMinScale"), generatorSettings.minScale, 0.005f, 0.0f, 1.0f);

            gui.displayText("Max scale");
            gui.dragFloat(gui::createHiddenLabel("pfxMaxScale"), generatorSettings.maxScale, 0.005f, 0.0f, 1.0f);

            gui.displayText("Delta scale");
            gui.dragFloat(gui::createHiddenLabel("pfxDeltaScale"), generatorSettings.deltaScale, 0.001f, 0.0f, 1.0f);

            gui.displayText("Direction factor");
            gui.dragFloat(gui::createHiddenLabel("pfxDirectionFactor"), generatorSettings.directionFactor, 0.01f, -5.0f, 5.0f);

            gui.popTreeNode();
        }

        gui.popId();
    }

    math::Vec3 position;
    int maxParticles;
    physx::pfx::ParticleGeneratorSettings pfxGeneratorSettings;
    std::vector<physx::pfx::Particle> particles;
};
}
