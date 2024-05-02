#pragma once

#include <optional>
#include <string>

#include "starlight/core/math/Core.h"
#include "starlight/core/FileSystem.h"

#include "starlight/renderer/Material.h"

namespace sl {

using namespace std::string_literals;

struct MaterialConfig {
    // TODO: pass it from outside
    inline static auto defaultDiffuseColor = Vec4f{ 1.0f };
    inline static auto defaultDiffuseMap   = "Internal.Texture.Default"s;
    inline static auto defaultNormalMap    = "Internal.Texture.DefaultNormalMap"s;
    inline static auto defaultSpecularMap  = "Internal.Texture.DefaultSpecularMap"s;
    inline static auto defaultShader       = "Builtin.Shader.Material"s;
    inline static auto defaultShininess    = 32.0f;

    inline static const std::string baseMaterialsPath =
      SL_ASSETS_PATH + std::string("/materials");

    static MaterialConfig createDefault(const std::string& name);

    static std::optional<MaterialConfig> load(
      const std::string& name, const std::string& materialsPath = baseMaterialsPath,
      const FileSystem& fs = fileSystem
    );

    std::string name;

    Vec4f diffuseColor;
    float shininess;
    std::string diffuseMap;
    std::string specularMap;
    std::string normalMap;

    std::string shaderName;
};

}  // namespace sl
