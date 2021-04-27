#pragma once

#include <string>

#include "sl/asset/AssetManager.h"
#include "sl/core/Json.h"
#include "sl/gui/GuiApi.h"
#include "sl/gui/Utils.hpp"
#include "sl/gui/fonts/FontAwesome.h"

#include "imgui/imgui.h"

namespace sl::ecs {

struct Component {
    virtual void onGui([[maybe_unused]] gui::GuiApi& gui, [[maybe_unused]] asset::AssetManager& assetManager) {
    }

    virtual void serialize([[maybe_unused]] core::JsonBuilder& builder) {
    }

    std::string ownerEntityId;
    bool isActive = true;
    bool shouldBeRemoved = false;
    std::string name;

    bool beginComponentTreeNode(gui::GuiApi& gui, const std::string& name) {
        gui.separator();

        bool isOpened = gui.beginTreeNode(name);

        gui.sameLine();
        gui.setFontScale(0.6f);
        gui.checkbox("##" + name, isActive);

        gui.setFontScale(1.0f);

        gui.sameLine(gui.getCurrentWindowWidth() - 35);
        gui.displayText(ICON_FA_TIMES);

        shouldBeRemoved = gui.isPreviousWidgetClicked();
        return isOpened;
    }

    static void serializeVector(core::JsonBuilder& builder, const std::string& name, const math::Vec3& vector) {
        builder.addField(name, std::vector<float> { vector.x, vector.y, vector.z });
    }

    static math::Vec3 deserializeVector3(Json::Value& value) {
        return math::Vec3 {
            value[0].asFloat(), value[1].asFloat(), value[2].asFloat()
        };
    }
};

}
