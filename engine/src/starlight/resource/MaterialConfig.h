#pragma once

#include <optional>

#include "starlight/core/math/Glm.h"
#include "starlight/core/FileSystem.h"

#include "Resource.h"

namespace sl {

struct MaterialConfig : public Resource {
    using Resource::Resource;

    static std::optional<MaterialConfig> create(
        const std::string& name, const FileSystem& fs = fileSystem
    );

    Vec4f diffuseColor;
    std::string diffuseMap;
};

}  // namespace sl
