#pragma once

#include "gpu/Texture.h"
#include "gpu/Mesh.h"
#include "gpu/Shader.h"

#include "starlight/renderer/Resource.hpp"

namespace sl {

class Skybox {
public:
    explicit Skybox(
      ResourceRef<Texture> cubeMap, ResourceRef<Mesh> mesh,
      ResourceRef<Shader> shader
    );
    ~Skybox();

    Skybox(Skybox& oth)            = delete;
    Skybox& operator=(Skybox& oth) = delete;

    u64 getId() const;
    Texture* getCubeMap();
    Mesh* getMesh();
    u32 getInstanceId() const;

    static ResourceRef<Skybox> load(const std::string& name);

private:
    u64 m_id;

    ResourceRef<Texture> m_cubeMap;
    ResourceRef<Mesh> m_mesh;
    ResourceRef<Shader> m_shader;

    u32 m_instanceId;
};

class SkyboxManager
    : public ResourceManager<Skybox>,
      public kc::core::Singleton<SkyboxManager> {
public:
    explicit SkyboxManager();

    ResourceRef<Skybox> load(const std::string& name);

private:
    ResourceRef<Mesh> m_cubeMesh;
    ResourceRef<Shader> m_defaultSkyboxShader;
};

}  // namespace sl
