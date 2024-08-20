#pragma once

#include "gpu/Texture.h"
#include "gpu/Mesh.h"
#include "gpu/Shader.h"

#include "starlight/renderer/Resource.hpp"

namespace sl {

class Skybox {
public:
    explicit Skybox(u64 id, Texture& cubeMap, Mesh& mesh, Shader& shader);
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

    Texture& m_cubeMap;
    Mesh& m_mesh;

    u32 m_instanceId;

    Shader& m_shader;
};

class SkyboxManager
    : public ResourceManager<Skybox>,
      public kc::core::Singleton<SkyboxManager> {
public:
    explicit SkyboxManager(RendererBackend& renderer);

    ResourceRef<Skybox> load(const std::string& name);

private:
    RendererBackend& m_renderer;

    ResourceRef<Mesh> m_cubeMesh;
    ResourceRef<Shader> m_defaultSkyboxShader;
};

}  // namespace sl
