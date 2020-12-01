#pragma once

#include "ParticleEffectComponent.h"
#include "sl/ecs/ComponentWrapper.h"
#include "sl/gui/Utils.hpp"

namespace sl::scene::components {

class ParticleEffectComponentWrapper : public ecs::ComponentWrapper {
public:
    using ecs::ComponentWrapper::ComponentWrapper;

    void onGUI(gui::Panel& panel) override {
        ParticleEffectComponent& component = static_cast<ParticleEffectComponent&>(m_component);
        if (panel.beginTreeNode("ParticleEffect")) {
            panel.displayText("Position");
            panel.dragFloat3(gui::createHiddenLabel("pfxPositon"), component.position, 0.1f);

            panel.displayText("Max particles");
            panel.dragInt(gui::createHiddenLabel("pfxMaxParticles"), component.maxParticles, 1.0, 1, 12500);

            auto& generatorSettings = component.pfxGeneratorSettings;

            panel.displayText("Radius");
            panel.dragFloat(gui::createHiddenLabel("pfxRadius"), generatorSettings.radius, 0.01f, 0.0f, 25.0f);

            panel.displayText("Min color");
            panel.dragFloat3(gui::createHiddenLabel("pfxMinColor"), generatorSettings.minColor, 0.01f, 0.0f, 1.0f);

            panel.displayText("Max color");
            panel.dragFloat3(gui::createHiddenLabel("pfxMaxColor"), generatorSettings.maxColor, 0.01f, 0.0f, 1.0f);

            panel.displayText("Min speed");
            panel.dragFloat(gui::createHiddenLabel("pfxMinSpeed"), generatorSettings.minSpeed, 0.005f, 0.0f, 15.0f);

            panel.displayText("Max speed");
            panel.dragFloat(gui::createHiddenLabel("pfxMaxSpeed"), generatorSettings.maxSpeed, 0.005f, 0.0f, 15.0f);

            panel.displayText("Min scale");
            panel.dragFloat(gui::createHiddenLabel("pfxMinScale"), generatorSettings.minScale, 0.005f, 0.0f, 1.0f);

            panel.displayText("Max scale");
            panel.dragFloat(gui::createHiddenLabel("pfxMaxScale"), generatorSettings.maxScale, 0.005f, 0.0f, 1.0f);

            panel.displayText("Delta scale");
            panel.dragFloat(gui::createHiddenLabel("pfxDeltaScale"), generatorSettings.deltaScale, 0.001f, 0.0f, 1.0f);

            panel.displayText("Direction factor");
            panel.dragFloat(gui::createHiddenLabel("pfxDirectionFactor"), generatorSettings.directionFactor, 0.01f, -5.0f, 5.0f);

            panel.popTreeNode();
        }
    }
};

struct ParticleEffectComponentWrapperFactory : public ecs::ComponentWrapperFactory {
    std::unique_ptr<ecs::ComponentWrapper> create(ecs::Component& component) override {
        return std::make_unique<ParticleEffectComponentWrapper>(component);
    }
};
}
