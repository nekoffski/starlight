#pragma once

#include <optional>
#include <string>

#include "starlight/core/math/Glm.h"

#include "gpu/Texture.h"
#include "gpu/Shader.h"
#include "fwd.h"

namespace sl {

class Material {
public:
    struct Properties {
        Vec4f diffuseColor;
        TextureMap* diffuseMap;
        TextureMap* specularMap;
        TextureMap* normalMap;
        float shininess;
        std::string name;
    };

    explicit Material(const Properties& props, u32 id, Shader& shader);
    ~Material();

    bool isTransparent() const;
    void applyUniforms(u32 frameNumber);

    u32 getId() const;
    const std::string& getName() const;

    void destroyTextureMaps(ResourcePools& resourcePools);

private:
    Properties m_props;
    Shader& m_shader;

    u32 m_id;
    u32 m_renderFrameNumber;
    u32 m_instanceId;
};

}  // namespace sl
