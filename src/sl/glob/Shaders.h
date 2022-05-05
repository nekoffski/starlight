#pragma once

#include <memory>
#include <unordered_map>

#include "sl/gpu/fwd.h"

namespace sl::glob {

struct Shaders {
    explicit Shaders();

    std::shared_ptr<gpu::Shader> defaultCubemapShader;
    std::shared_ptr<gpu::Shader> defaultModelShader;
    std::shared_ptr<gpu::Shader> singleColorShader;
    std::shared_ptr<gpu::Shader> pfxShader;

    std::unordered_map<std::string, std::shared_ptr<gpu::Shader>&> shadersByName;
};
}  // namespace sl::glob
