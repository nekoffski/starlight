#pragma once

#include <memory>

#include "sl/graphics/fwd.h"

namespace sl::utils::globals {

struct Shaders {
    explicit Shaders();

    std::shared_ptr<graphics::Shader> defaultCubemapShader;
    std::shared_ptr<graphics::Shader> defaultModelShader;
};
}
