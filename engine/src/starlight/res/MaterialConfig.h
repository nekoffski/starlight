#pragma once

#include <optional>

#include "starlight/math/Glm.h"
#include "starlight/core/FileSystem.h"

#include "Resource.h"

namespace sl::res {

struct MaterialConfig : public Resource {
    using Resource::Resource;

    static std::optional<MaterialConfig> create(
        const std::string& name, const core::FileSystem& fs = core::fileSystem
    );

    math::Vec4f diffuseColor;
    std::string diffuseMap;
};

}  // namespace sl::res
