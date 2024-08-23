#pragma once

#pragma once

#include "starlight/core/Core.hh"
#include "starlight/core/ResourcePool.hh"

#include "starlight/renderer/Skybox.hh"

#include "TextureManager.hh"
#include "ResourceManager.hh"
#include "MeshManager.hh"

namespace sl {

class SkyboxManager : public ResourceManager<Skybox, SkyboxManager> {
    static constexpr u64 maxSkyboxes = 128;

public:
    explicit SkyboxManager(
      ResourcePools& resourcePools, TextureManager& textureManager,
      MeshManager& meshManager
    );
    ~SkyboxManager();

    Skybox* load(const std::string& name, Shader& skyboxShader);

private:
    void destroyInternals(Skybox* resource) override;

    ResourcePools& m_resourcePools;
    TextureManager& m_textureManager;

    ResourcePool<Skybox> m_skyboxes;

    Mesh* m_skyboxMesh;
};

}  // namespace sl
