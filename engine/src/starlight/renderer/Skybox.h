#pragma once

#include "gpu/Texture.h"
#include "gpu/Mesh.h"
#include "gpu/Shader.h"

namespace sl {

class Skybox {
public:
    explicit Skybox(TextureMap* cubeMap, Mesh* mesh, Shader& shader);
    ~Skybox();

    Skybox(Skybox& oth)            = delete;
    Skybox& operator=(Skybox& oth) = delete;

    TextureMap* cubeMap;
    Mesh* mesh;
    const u32 instanceId;

private:
    Shader& m_shader;
};

}  // namespace sl
