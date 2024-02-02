#pragma once

#include <string>

#include "Utils.h"

#include "starlight/core/Core.h"
#include "starlight/core/math/Glm.h"

namespace sl::ui {

struct Widget {
    virtual void render() = 0;
};

}  // namespace sl::ui
