#pragma once

#include "sl/core/Colors.h"
#include "sl/ecs/Component.h"
#include "sl/ecs/Entity.h"
#include "sl/math/Vector.hpp"

namespace sl::scene::components {

struct MaterialComponent : ecs::Component {
    explicit MaterialComponent(math::Vec3 ambientColor = core::colorWhite, math::Vec3 diffuseColor = core::colorWhite,
        math::Vec3 specularColor = core::colorWhite, float shininess = 32.0f)
        : ambientColor(ambientColor)
        , diffuseColor(diffuseColor)
        , specularColor(specularColor)
        , shininess(shininess) {
    }

    void onGui(gui::GuiApi& gui) override {
        if (gui.beginTreeNode("Material")) {
            gui.displayText("Ambient color");
            gui.colorPicker3(gui::createHiddenLabel("Ambient color"), ambientColor);
            gui.displayText("Diffuse color");
            gui.colorPicker3(gui::createHiddenLabel("Diffuse color"), diffuseColor);
            gui.displayText("Specular color");
            gui.colorPicker3(gui::createHiddenLabel("Specular color"), specularColor);
            gui.displayText("Shininess");
            gui.dragFloat(gui::createHiddenLabel("rotation"), shininess, 0.5f, 0.0f, 128.0f);
            gui.popTreeNode();
        }
    }

    void serialize(core::JsonBuilder& builder) override {
        builder.addField("name", "MaterialComponent"s);
        serializeVector(builder, "ambient-color", ambientColor);
        serializeVector(builder, "diffuse-color", diffuseColor);
        serializeVector(builder, "specular-color", specularColor);
        builder.addField("shininess", shininess);
    }

    static void deserialize(std::shared_ptr<ecs::Entity> entity, asset::AssetManager& assetManager, Json::Value& componentDescription) {
        entity->addComponent<MaterialComponent>(
            deserializeVector3(componentDescription["ambient-color"]),
            deserializeVector3(componentDescription["diffuse-color"]),
            deserializeVector3(componentDescription["specular-color"]),
            componentDescription["shininess"].asFloat());
    }

    math::Vec3 ambientColor;
    math::Vec3 diffuseColor;
    math::Vec3 specularColor;

    float shininess;
};
}
