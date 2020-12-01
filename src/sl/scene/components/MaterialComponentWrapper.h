#pragma once

#include "MaterialComponent.h"
#include "sl/ecs/ComponentWrapper.h"
#include "sl/gui/Utils.hpp"

namespace sl::scene::components {

class MaterialComponentWrapper : public ecs::ComponentWrapper {
public:
    using ecs::ComponentWrapper::ComponentWrapper;

    void onGUI(gui::Panel& panel) override {
        MaterialComponent& component = static_cast<MaterialComponent&>(m_component);
        if (panel.beginTreeNode("Material")) {
            panel.displayText("Ambient color");
            panel.colorPicker3(gui::createHiddenLabel("Ambient color"), component.ambientColor);
            panel.displayText("Diffuse color");
            panel.colorPicker3(gui::createHiddenLabel("Diffuse color"), component.diffuseColor);
            panel.displayText("Specular color");
            panel.colorPicker3(gui::createHiddenLabel("Specular color"), component.specularColor);
            panel.displayText("Shininess");
            panel.dragFloat(gui::createHiddenLabel("rotation"), component.shininess, 0.5f, 0.0f, 128.0f);
            panel.popTreeNode();
        }
    }
};

struct MaterialComponentWrapperFactory : public ecs::ComponentWrapperFactory {
    std::unique_ptr<ecs::ComponentWrapper> create(ecs::Component& component) override {
        return std::make_unique<MaterialComponentWrapper>(component);
    }
};
}
