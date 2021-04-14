#pragma once

#include <string>

#include "sl/asset/AssetManager.h"
#include "sl/core/Json.h"
#include "sl/gui/GuiApi.h"
#include "sl/gui/Utils.hpp"

namespace sl::ecs {

struct Component {
    virtual void onGui([[maybe_unused]] gui::GuiApi& gui) { }
    virtual void serialize([[maybe_unused]] core::JsonBuilder& builder) { }

    std::string ownerEntityId;
    bool isActive = true;

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
