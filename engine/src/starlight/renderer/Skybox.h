#pragma once

#include "gpu/Texture.h"
#include "gpu/Geometry.h"
#include "gpu/Shader.h"

namespace sl {

class Skybox {
public:
    explicit Skybox(TextureMap* cubeMap, Geometry* geometry, Shader& shader);
    ~Skybox();

    Skybox(Skybox& oth)            = delete;
    Skybox& operator=(Skybox& oth) = delete;

    TextureMap* cubeMap;
    Geometry* geometry;
    const u32 instanceId;

private:
    Shader& m_shader;
};

}  // namespace sl
