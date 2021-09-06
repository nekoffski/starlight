#pragma once

#include <string>

#include "sl/asset/AssetManager.h"
#include "sl/gui/GuiApi.h"
#include "sl/gui/Utils.hpp"
#include "sl/gui/fonts/FontAwesome.h"

#include "imgui/imgui.h"

namespace sl::ecs {

struct Component {
    std::string ownerEntityId;
    bool isActive = true;
    bool shouldBeRemoved = false;
    std::string name;
};

}
