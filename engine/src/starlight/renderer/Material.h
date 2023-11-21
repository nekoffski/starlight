#pragma once

#include <optional>
#include <string>

#include "starlight/core/math/Glm.h"

#include "Texture.h"
#include "Shader.h"
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
