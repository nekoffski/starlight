#pragma once

#include <span>

#include "Model.h"

namespace sl {

struct RenderPacket {
    std::vector<Model*> models;
};

}  // namespace sl
