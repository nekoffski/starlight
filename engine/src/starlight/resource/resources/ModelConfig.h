#pragma once

#include <string>
#include <optional>
#include <vector>

#include "starlight/core/utils/FileSystem.h"
#include "GeometryConfig.h"
#include "MaterialConfig.h"

namespace sl {

struct ModelConfig {
    inline static const std::string baseModelsPath =
      SL_ASSETS_PATH + std::string("/models");

    static std::optional<ModelConfig> loadOBJ(
      const std::string& name, const std::string& modelsPath = baseModelsPath,
      const FileSystem& fs = fileSystem
    );

    std::vector<MaterialConfig> materials;
    std::vector<GeometryConfig3D> geometries;
};

}  // namespace sl