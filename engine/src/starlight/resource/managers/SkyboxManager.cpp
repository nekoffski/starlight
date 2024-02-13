#include "SkyboxManager.h"

#include "starlight/renderer/gpu/ResourcePools.h"

#include "starlight/resource/configs/MeshConfig.h"

namespace sl {

SkyboxManager::SkyboxManager(
  ResourcePools& resourcePools, TextureManager& textureManager,
  MeshManager& meshManager
) :
    ResourceManager("Skybox"),
    m_resourcePools(resourcePools), m_textureManager(textureManager),
    m_skyboxes("Skybox", maxSkyboxes),
    m_skyboxMesh(meshManager.load(MeshConfig3D::generateCube(CubeProperties{
      10.0f, 10.0f, 10.0f, 1, 1, "Internal.Mesh.SkyboxCube" }))) {}

SkyboxManager::~SkyboxManager() {
    forEach([&](u64 id, Skybox* skybox) {
        m_resourcePools.destroyTextureMap(*skybox->getCubeMap());
        m_skyboxes.destroy(id);
    });
}

Skybox* SkyboxManager::load(const std::string& name, Shader& skyboxShader) {
    TextureMap::Properties cubeMapProps{ TextureMap::Use::cubeMap };
    auto cubeTexture = m_textureManager.loadCubeTexture(name);
    auto cubeMap     = m_resourcePools.createTextureMap(cubeMapProps, *cubeTexture);

    auto skybox = m_skyboxes.create(*cubeMap, *m_skyboxMesh, skyboxShader);
    return storeResource(name, skybox->getId(), skybox);
}

void SkyboxManager::destroyInternals(Skybox* skybox) {
    m_resourcePools.destroyTextureMap(*skybox->getCubeMap());
    m_skyboxes.destroy(skybox->getId());
}

}  // namespace sl
