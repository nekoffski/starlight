#pragma once

#include "sl/core/misc/colors.hpp"
#include "sl/ecs/Component.h"
#include "sl/ecs/Entity.h"
#include "sl/graphics/Texture.h"
#include "sl/math/Matrix.hpp"
#include "sl/math/Vector.hpp"

namespace sl::scene::components {

static const auto lightProjectionMatrix = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f);

struct DirectionalLightComponent : ecs::Component {
    explicit DirectionalLightComponent(math::Vec3 direction = math::Vec3{ 1.0f, 1.0f, 1.0f }, math::Vec3 color = core::misc::COL_WHITE)
        : direction(direction)
        , color(color)
        , shadowMap(graphics::Texture::factory->create(1024u, 1024u))
        , viewMatrix(math::lookAt(direction, math::Vec3{ 0.0f }, math::Vec3{ 0.0f, 1.0f, 0.0f }))
        , spaceMatrix(lightProjectionMatrix * viewMatrix) {
    }

    void onGui(gui::GuiApi& gui) override {
        if (gui.beginTreeNode("Directional light")) {
            gui.displayText("Position");

            if (gui.dragFloat3(gui::createHiddenLabel("dlcDirection"), direction, 0.01f, -1.0f, 1.0f)) {
                viewMatrix = math::lookAt(direction, math::Vec3{ 0.0f }, math::Vec3{ 0.0f, 1.0f, 0.0f });
                spaceMatrix = lightProjectionMatrix * viewMatrix;
            }

            gui.displayText("Color");
            gui.colorPicker3(gui::createHiddenLabel("dlcColor"), color);

            gui.popTreeNode();
        }
    }

    void serialize(core::JsonBuilder& builder) override {
        builder.addField("name", "DirectionalLightComponent"s);
        serializeVector(builder, "direction", direction);
        serializeVector(builder, "color", color);
    }

    static void deserialize(std::shared_ptr<ecs::Entity> entity, asset::AssetManager& assetManager, Json::Value& componentDescription) {
        entity->addComponent<DirectionalLightComponent>(
            deserializeVector3(componentDescription["direction"]),
            deserializeVector3(componentDescription["color"]));
    }

    math::Vec3 direction;
    math::Vec3 color;
    math::Mat4 viewMatrix;
    math::Mat4 spaceMatrix;
    std::shared_ptr<sl::graphics::Texture> shadowMap;
};
}
