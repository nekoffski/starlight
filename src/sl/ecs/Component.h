#pragma once

#include <string>

#include "sl/gui/GuiApi.h"
#include "sl/gui/Utils.hpp"

namespace sl::ecs {

struct Component {
    virtual void onGui([[maybe_unused]] gui::GuiApi& gui) {}

    std::string ownerEntityId;
};
}
