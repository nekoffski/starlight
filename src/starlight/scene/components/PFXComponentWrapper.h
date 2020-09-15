#pragma once

#include "PFXComponent.h"
#include "starlight/ecs/ComponentWrapper.h"
#include "starlight/gui/Utils.hpp"

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
