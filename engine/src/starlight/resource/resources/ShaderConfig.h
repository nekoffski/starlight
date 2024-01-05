#pragma once

#include <string>
#include <vector>

#include "starlight/core/Core.h"
#include "starlight/core/utils/FileSystem.h"
#include "starlight/renderer/gpu/Shader.h"

namespace sl {

struct ShaderConfig {
    inline static const std::string baseShaderPath =
      SL_ASSETS_PATH + std::string("/shaders");

    static std::optional<ShaderConfig> load(
      const std::string& name, Texture* defaultTexture,
      const std::string& shadersPath = baseShaderPath,
      const FileSystem& fs           = fileSystem
    );

    Shader::Properties properties;
};

}  // namespace sl
