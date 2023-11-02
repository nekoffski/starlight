#pragma once

#include <optional>

#include "starlight/core/math/Glm.h"
#include "starlight/core/utils/FileSystem.h"

#include "starlight/renderer/Material.h"

namespace sl {

struct MaterialConfig {
    inline static const std::string baseMaterialsPath =
      SL_ASSETS_PATH + std::string("/materials");

    static std::optional<MaterialConfig> load(
      const std::string& name, const std::string& materialsPath = baseMaterialsPath,
      const FileSystem& fs = fileSystem
    );

    Vec4f diffuseColor;
    float shininess;
    std::string diffuseMap;
    std::string specularMap;
    std::string normalMap;
    std::string shaderName;
};

}  // namespace sl
