#pragma once

#include "PFXComponent.h"
#include "sl/ecs/ComponentWrapper.h"
#include "sl/gui/Utils.hpp"

namespace sl::scene::components {

class PFXComponentWrapper : public ecs::ComponentWrapper {
public:
    using ecs::ComponentWrapper::ComponentWrapper;

    void onGUI(gui::Window& window) override {
        PFXComponent& component = static_cast<PFXComponent&>(m_component);
        if (window.beginTreeNode("PFX")) {
            window.displayText("Position");
            window.dragFloat3(gui::createHiddenLabel("pfxPositon"), component.position, 0.1f);

            window.displayText("Max particles");
            window.dragInt(gui::createHiddenLabel("pfxMaxParticles"), component.maxParticles, 1.0, 1, 12500);

            auto& generatorSettings = component.pfxGeneratorSettings;

            window.displayText("Radius");
            window.dragFloat(gui::createHiddenLabel("pfxRadius"), generatorSettings.radius, 0.01f, 0.0f, 25.0f);

            window.displayText("Min color");
            window.dragFloat3(gui::createHiddenLabel("pfxMinColor"), generatorSettings.minColor, 0.01f, 0.0f, 1.0f);

            window.displayText("Max color");
            window.dragFloat3(gui::createHiddenLabel("pfxMaxColor"), generatorSettings.maxColor, 0.01f, 0.0f, 1.0f);

            window.displayText("Min speed");
            window.dragFloat(gui::createHiddenLabel("pfxMinSpeed"), generatorSettings.minSpeed, 0.005f, 0.0f, 15.0f);

            window.displayText("Max speed");
            window.dragFloat(gui::createHiddenLabel("pfxMaxSpeed"), generatorSettings.maxSpeed, 0.005f, 0.0f, 15.0f);

            window.displayText("Min scale");
            window.dragFloat(gui::createHiddenLabel("pfxMinScale"), generatorSettings.minScale, 0.005f, 0.0f, 1.0f);

            window.displayText("Max scale");
            window.dragFloat(gui::createHiddenLabel("pfxMaxScale"), generatorSettings.maxScale, 0.005f, 0.0f, 1.0f);

            window.displayText("Delta scale");
            window.dragFloat(gui::createHiddenLabel("pfxDeltaScale"), generatorSettings.deltaScale, 0.001f, 0.0f, 1.0f);

            window.displayText("Direction factor");
            window.dragFloat(gui::createHiddenLabel("pfxDirectionFactor"), generatorSettings.directionFactor, 0.01f, -5.0f, 5.0f);

            window.popTreeNode();
        }
    }
};

struct PFXComponentWrapperFactory : public ecs::ComponentWrapperFactory {
    std::unique_ptr<ecs::ComponentWrapper> create(ecs::Component& component) override {
        return std::make_unique<PFXComponentWrapper>(component);
    }
};
}
