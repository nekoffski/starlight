#pragma once

#include <string>
#include <optional>

#include "starlight/core/Core.h"
#include "starlight/core/utils/FileSystem.h"

#include "resources/MaterialConfig.h"
#include "resources/ImageData.h"
#include "resources/ShaderConfig.h"

namespace sl {

class ResourceLoader {
public:
    explicit ResourceLoader(
      const std::string& baseResourcePath = SL_ASSETS_PATH,
      FileSystem& fs                      = fileSystem
    );

    std::optional<MaterialConfig> loadMaterialConfig(const std::string& name) const;
    std::optional<STBImageData> loadImageData(const std::string& name) const;
    std::optional<ShaderConfig> loadShaderConfig(const std::string& name) const;

private:
    std::string m_baseResourcePath;
    FileSystem& m_fs;
};

}  // namespace sl