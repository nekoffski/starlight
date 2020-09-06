#pragma once

#include "MaterialComponent.h"
#include "starlight/ecs/ComponentWrapper.h"
#include "starlight/gui/Utils.hpp"

namespace sl::scene::components {

class MaterialComponentWrapper : public ecs::ComponentWrapper {
public:
    using ecs::ComponentWrapper::ComponentWrapper;

    void onGUI(gui::Window& window) override {
        MaterialComponent& component = static_cast<MaterialComponent&>(m_component);
        if (window.beginTreeNode("Material")) {
            window.displayText("Ambient color");
            window.colorPicker3(gui::createHiddenLabel("Ambient color"), component.ambientColor);
            window.displayText("Diffuse color");
            window.colorPicker3(gui::createHiddenLabel("Diffuse color"), component.diffuseColor);
            window.displayText("Specular color");
            window.colorPicker3(gui::createHiddenLabel("Specular color"), component.specularColor);
            window.displayText("Shininess");
            window.dragFloat(gui::createHiddenLabel("rotation"), component.shininess, 0.5f, 0.0f, 128.0f);
            window.popTreeNode();
        }
    }
};

struct MaterialComponentWrapperFactory : public ecs::ComponentWrapperFactory {
    std::unique_ptr<ecs::ComponentWrapper> create(ecs::Component& component) override {
        return std::make_unique<MaterialComponentWrapper>(component);
    }
};
}
