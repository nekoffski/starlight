#pragma once

#include <optional>
#include <string>

#include "starlight/core/math/Glm.h"

#include "Texture.h"
#include "TextureMap.h"
#include "Shader.h"

#include "starlight/core/utils/Id.h"

namespace sl {

struct Material {
    uint32_t generation;
    uint32_t internalId;
    std::string name;
    Vec4f diffuseColor;
    TextureMap diffuseMap;
    Shader* shader;

    Id id = idGenerator++;

    inline static Id idGenerator = 0;

    void acquireInstanceResources() {
        internalId = shader->acquireInstanceResources();
        LOG_ERROR("{} - Acquired instance resources id = {}", name, internalId);
    }

    void releaseInstanceResources() {
        shader->releaseInstanceResources(internalId);
        internalId = 0;
    }
};

}  // namespace sl
