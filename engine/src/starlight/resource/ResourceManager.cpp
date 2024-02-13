// #include "ResourceManager.h"

// #include "resources/ModelConfig.h"

// namespace sl {

// ResourceManager::ResourceManager(ResourcePools& resourcePools) :
//     m_resourcePools(resourcePools), m_textureManager(resourcePools),
//     m_shaderManager(resourcePools, m_textureManager),
//     m_materialManager(m_shaderManager, m_textureManager, resourcePools),
//     m_meshManager(resourcePools, m_materialManager) {
//     m_materialManager.createDefaultMaterial();
//     m_skyboxMesh = loadMesh(MeshConfig3D::generateCube(CubeProperties{
//       10.0f, 10.0f, 10.0f, 1, 1, "Internal.Mesh.SkyboxCube" }));
// }

// Shader* ResourceManager::loadShader(const std::string& name) {
//     return m_shaderManager.load(name);
// }

// Texture* ResourceManager::loadTexture(const std::string& name) {
//     return m_textureManager.load(name);
// }

// Texture* ResourceManager::loadCubeTexture(const std::string& name) {
//     return m_textureManager.loadCubeTexture(name);
// }

// Texture* ResourceManager::acquireTexture(const std::string& name) const {
//     return m_textureManager.acquire(name);
// }

// Texture* ResourceManager::acquireTexture(u64 id) const {
//     return m_textureManager.acquire(id);
// }

// void ResourceManager::destroyTexture(const std::string& name) {
//     m_textureManager.destroy(name);
// }

// Material* ResourceManager::loadMaterial(const MaterialConfig& config) {
//     return m_materialManager.load(config);
// }

// Material* ResourceManager::loadMaterial(const std::string& name) {
//     return m_materialManager.load(name);
// }

// Material* ResourceManager::acquireMaterial(const std::string& name) {
//     return m_materialManager.acquire(name);
// }

// Material* ResourceManager::getDefaultMaterial() {
//     return m_materialManager.getDefaultMaterial();
// }

// UniqPtr<Skybox> ResourceManager::loadSkybox(
//   const std::string& name, Shader& skyboxShader
// ) {
//     TextureMap::Properties cubeMapProps{ TextureMap::Use::cubeMap };
//     auto cubeTexture = loadCubeTexture(name);
//     auto cubeMap     = m_resourcePools.createTextureMap(cubeMapProps,
//     *cubeTexture); return createUniqPtr<Skybox>(cubeMap, m_skyboxMesh,
//     skyboxShader);
// }

// std::optional<Model> ResourceManager::loadModel(const std::string& path) {
//     auto config = ModelConfig::loadOBJ(path);

//     if (not config) {
//         LOG_ERROR("Could not load model: '{}'", path);
//         return {};
//     }

//     for (auto& material : config->materials) loadMaterial(material);

//     Model model;
//     model.meshes.reserve(config->meshes.size());

//     std::transform(
//       config->meshes.begin(), config->meshes.end(),
//       std::back_inserter(model.meshes),
//       [&](MeshConfig3D& config) -> Mesh* { return loadMesh(config); }
//     );

//     return model;
// }

// void ResourceManager::destroyMaterial(const std::string& name) {
//     m_materialManager.destroy(name);
// }

// Mesh* ResourceManager::acquireMesh(uint32_t id) { return
// m_meshManager.acquire(id); }

// void ResourceManager::destroyMesh(uint32_t id) { m_meshManager.destroy(id); }

// Mesh* ResourceManager::getDefaultMesh3D() { return m_meshManager.getDefault3D(); }

// Mesh* ResourceManager::getDefaultMesh2D() { return m_meshManager.getDefault2D(); }

// }  // namespace sl
