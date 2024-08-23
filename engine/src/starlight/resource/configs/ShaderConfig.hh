#pragma once

#include <string>
#include <vector>

#include "starlight/core/Core.hh"
#include "starlight/core/FileSystem.hh"
#include "starlight/renderer/gpu/Shader.hh"

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
