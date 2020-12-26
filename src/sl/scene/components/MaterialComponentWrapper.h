#pragma once

#include "MaterialComponent.h"
#include "sl/ecs/ComponentWrapper.h"
#include "sl/gui/Utils.hpp"

namespace sl::scene::components {

class MaterialComponentWrapper : public ecs::ComponentWrapper {
public:
    using ecs::ComponentWrapper::ComponentWrapper;

    void onGui(gui::GuiApi& gui) override {
        MaterialComponent& component = static_cast<MaterialComponent&>(m_component);
        if (gui.beginTreeNode("Material")) {
            gui.displayText("Ambient color");
            gui.colorPicker3(gui::createHiddenLabel("Ambient color"), component.ambientColor);
            gui.displayText("Diffuse color");
            gui.colorPicker3(gui::createHiddenLabel("Diffuse color"), component.diffuseColor);
            gui.displayText("Specular color");
            gui.colorPicker3(gui::createHiddenLabel("Specular color"), component.specularColor);
            gui.displayText("Shininess");
            gui.dragFloat(gui::createHiddenLabel("rotation"), component.shininess, 0.5f, 0.0f, 128.0f);
            gui.popTreeNode();
        }
    }
};

struct MaterialComponentWrapperFactory : public ecs::ComponentWrapperFactory {
    std::unique_ptr<ecs::ComponentWrapper> create(ecs::Component& component) override {
        return std::make_unique<MaterialComponentWrapper>(component);
    }
};
}
