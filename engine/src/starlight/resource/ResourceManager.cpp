#include "ResourceManager.h"

#include "resources/MeshConfig.h"

namespace sl {

ResourceManager::ResourceManager(ResourcePools& resourcePools) :
    m_resourcePools(resourcePools), m_textureManager(resourcePools),
    m_shaderManager(resourcePools, m_textureManager),
    m_materialManager(m_shaderManager, m_textureManager, resourcePools),
    m_geometryManager(resourcePools, m_materialManager) {
    m_materialManager.createDefaultMaterial();
    m_skyboxGeometry = loadGeometry(GeometryConfig3D::generateCube(CubeProperties{
      10.0f, 10.0f, 10.0f, 1, 1, "Internal.Geometry.SkyboxCube" }));
}

Shader* ResourceManager::loadShader(const std::string& name) {
    return m_shaderManager.load(name);
}

Texture* ResourceManager::loadTexture(const std::string& name) {
    return m_textureManager.load(name);
}

Texture* ResourceManager::loadCubeTexture(const std::string& name) {
    return m_textureManager.loadCubeTexture(name);
}

Texture* ResourceManager::acquireTexture(const std::string& name) const {
    return m_textureManager.acquire(name);
}

void ResourceManager::destroyTexture(const std::string& name) {
    m_textureManager.destroy(name);
}

Material* ResourceManager::loadMaterial(const MaterialConfig& config) {
    return m_materialManager.load(config);
}

Material* ResourceManager::loadMaterial(const std::string& name) {
    return m_materialManager.load(name);
}

Material* ResourceManager::acquireMaterial(const std::string& name) {
    return m_materialManager.acquire(name);
}

Material* ResourceManager::getDefaultMaterial() {
    return m_materialManager.getDefaultMaterial();
}

UniqPtr<Skybox> ResourceManager::loadSkybox(
  const std::string& name, Shader& skyboxShader
) {
    TextureMap::Properties cubeMapProps{ TextureMap::Use::cubeMap };
    auto cubeTexture = loadCubeTexture(name);
    auto cubeMap     = m_resourcePools.createTextureMap(cubeMapProps, *cubeTexture);
    return createUniqPtr<Skybox>(cubeMap, m_skyboxGeometry, skyboxShader);
}

std::optional<Mesh> ResourceManager::loadMesh(const std::string& path) {
    auto config = MeshConfig::loadOBJ(path);

    if (not config) {
        LOG_ERROR("Could not load mesh: '{}'", path);
        return {};
    }

    for (auto& material : config->materials) loadMaterial(material);

    Mesh mesh;
    mesh.geometries.reserve(config->geometries.size());

    std::transform(
      config->geometries.begin(), config->geometries.end(),
      std::back_inserter(mesh.geometries),
      [&](GeometryConfig3D& config) -> Geometry* { return loadGeometry(config); }
    );

    return mesh;
}

void ResourceManager::destroyMaterial(const std::string& name) {
    m_materialManager.destroy(name);
}

Geometry* ResourceManager::acquireGeometry(uint32_t id) {
    return m_geometryManager.acquire(id);
}

void ResourceManager::destroyGeometry(uint32_t id) { m_geometryManager.destroy(id); }

Geometry* ResourceManager::getDefaultGeometry3D() {
    return m_geometryManager.getDefault3D();
}

Geometry* ResourceManager::getDefaultGeometry2D() {
    return m_geometryManager.getDefault2D();
}

}  // namespace sl
