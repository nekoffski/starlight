#pragma once

#include <optional>

#include "nova/math/Glm.h"
#include "nova/core/FileSystem.h"

#include "Resource.h"

namespace nova::res {

struct MaterialConfig : public Resource {
    using Resource::Resource;

    static std::optional<MaterialConfig> create(
        const std::string& name, const core::FileSystem& fs = core::fileSystem
    );

    math::Vec4f diffuseColor;
    std::string diffuseMap;
};

}  // namespace nova::res
