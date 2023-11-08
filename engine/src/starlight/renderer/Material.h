#pragma once

#include <optional>
#include <string>

#include "starlight/core/math/Glm.h"

#include "Texture.h"
#include "Shader.h"

#include "starlight/core/utils/Id.h"

namespace sl {

struct Material {
    u32 generation;
    u32 internalId;
    std::string name;
    Vec4f diffuseColor;
    TextureMap diffuseMap;
    TextureMap specularMap;
    TextureMap normalMap;
    Shader* shader;
    float shininess;
    Id32 renderFrameNumber;

    // TODO: CRTP
    Id id                        = idGenerator++;
    inline static Id idGenerator = 0;

    void applyUniforms(Shader* shader);
    void acquireInstanceResources();
    void releaseInstanceResources();
};

}  // namespace sl
