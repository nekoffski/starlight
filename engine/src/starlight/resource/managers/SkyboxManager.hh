#pragma once

#pragma once

#include "starlight/core/Core.h"
#include "starlight/core/ResourcePool.hpp"

#include "starlight/renderer/Skybox.h"

#include "TextureManager.h"
#include "ResourceManager.hpp"
#include "MeshManager.h"

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
