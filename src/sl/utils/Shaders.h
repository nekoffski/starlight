#pragma once

#include <memory>
#include <unordered_map>

#include "sl/gfx/fwd.h"

namespace sl::utils {

struct Shaders {
    explicit Shaders();

    std::shared_ptr<gfx::Shader> defaultCubemapShader;
    std::shared_ptr<gfx::Shader> defaultModelShader;
    std::shared_ptr<gfx::Shader> singleColorShader;
    std::shared_ptr<gfx::Shader> pfxShader;

    std::unordered_map<std::string, std::shared_ptr<gfx::Shader>&> shadersByName;
};
}
