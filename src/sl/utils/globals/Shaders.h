#pragma once

#include <memory>

#include "sl/gfx/fwd.h"

namespace sl::utils::globals {

struct Shaders {
    explicit Shaders();

    std::shared_ptr<gfx::Shader> defaultCubemapShader;
    std::shared_ptr<gfx::Shader> defaultModelShader;
};
}
