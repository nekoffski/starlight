#pragma once

#include <optional>
#include <string>

#include "starlight/core/math/Core.h"

#include "gpu/Texture.h"
#include "gpu/Shader.h"
#include "fwd.h"

namespace sl {

class Material {
public:
    struct Properties {
        Vec4<f32> diffuseColor;
        Texture* diffuseMap;
        Texture* specularMap;
        Texture* normalMap;
        float shininess;
        std::string name;
    };

    explicit Material(u64 id, const Properties& props, Shader& shader);
    ~Material();

    bool isTransparent() const;
    void applyUniforms(u64 frameNumber);

    u64 getId() const;
    const std::string& getName() const;
    const Properties& getProperties() const;

private:
    u64 m_id;

    Properties m_props;
    Shader& m_shader;

    u64 m_renderFrameNumber;
    u64 m_instanceId;
};

}  // namespace sl
