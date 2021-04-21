#pragma once

#include <memory>

#include "sl/core/Json.h"
#include "sl/ecs/Component.h"
#include "sl/ecs/Entity.h"
#include "sl/gui/Utils.hpp"
#include "sl/gui/fonts/FontAwesome.h"
#include "sl/math/Utils.hpp"
#include "sl/math/Vector.hpp"

namespace sl::scene::components {

struct TransformComponent : ecs::Component {
    explicit TransformComponent(math::Vec3 position = math::Vec3 { 0.0f },
        math::Vec3 rotation = math::Vec3 { 0.0f }, math::Vec3 scale = math::Vec3 { 1.0f })
        : position(position)
        , rotation(rotation)
        , scale(scale) {
        recalculate();
    }

    void onGui(gui::GuiApi& gui) override {
        gui.pushId(ownerEntityId);

        if (beginComponentTreeNode(gui, ICON_FA_STREET_VIEW "  Transform")) {
            int trigerred = 0;

            gui.displayText("Translation");
            trigerred += gui.dragFloat3(gui::createHiddenLabel("translation"), position, 0.1f);
            gui.displayText("Rotation");
            trigerred += gui.dragFloat3(gui::createHiddenLabel("rotation"), rotation, 0.5f, 0.0f, 360.0f);
            gui.displayText("Scale");
            trigerred += gui.dragFloat3(gui::createHiddenLabel("scale"), scale, 0.5f, 0.0f, 360.0f);
            gui.popTreeNode();

            if (trigerred > 0)
                recalculate();
        }

        gui.popId();
    }

    void serialize(core::JsonBuilder& builder) override {
        builder.addField("name", "TransformComponent"s);
        serializeVector(builder, "position", position);
        serializeVector(builder, "rotation", rotation);
        serializeVector(builder, "scale", scale);
    }

    static void deserialize(std::shared_ptr<ecs::Entity> entity, asset::AssetManager& assetManager, Json::Value& componentDescription) {
        entity->addComponent<TransformComponent>(
            deserializeVector3(componentDescription["position"]),
            deserializeVector3(componentDescription["rotation"]),
            deserializeVector3(componentDescription["scale"]));
    }

    math::Vec3 position;
    math::Vec3 rotation;
    math::Vec3 scale;

    math::Mat4 transformation;

    void recalculate() {
        transformation = math::scale(scale) * math::translate(position) * math::createRotationMatrix(math::toRadians(rotation));
    }

    math::Mat4 operator()() {
        return transformation;
    }
};
}
