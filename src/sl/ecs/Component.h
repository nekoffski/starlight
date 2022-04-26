#pragma once

#include <string>

#include "imgui/imgui.h"
#include "sl/asset/AssetManager.h"
#include "sl/gui/Utils.h"
#include "sl/gui/fonts/FontAwesome.h"

namespace sl::ecs {

struct Component {
    std::string ownerEntityId;
    bool isActive        = true;
    bool shouldBeRemoved = false;
    std::string name;
};

}  // namespace sl::ecs
