#include "ResourceManager.h"

namespace sl {

ResourceManager::ResourceManager(
  TextureLoader& textureLoader, RendererProxy& rendererProxy
) :
    m_textureManager(textureLoader, m_resourceLoader),
    m_materialManager(m_textureManager, rendererProxy, m_resourceLoader),
    m_geometryManager(rendererProxy, m_materialManager),
    m_shaderManager(rendererProxy, m_resourceLoader, m_textureManager) {}

Shader* ResourceManager::loadShader(const std::string& name) {
    return m_shaderManager.load(name);
}

Texture* ResourceManager::loadTexture(const std::string& name) {
    return m_textureManager.load(name);
}

Texture* ResourceManager::acquireTexture(const std::string& name) const {
    return m_textureManager.acquire(name);
}

Texture* ResourceManager::getDefaultTexture() const {
    return m_textureManager.getDefaultTexture();
}

void ResourceManager::destroyTexture(const std::string& name) {
    m_textureManager.destroy(name);
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

void ResourceManager::destroyMaterial(const std::string& name) {
    m_materialManager.destroy(name);
}

Geometry* ResourceManager::acquireGeometry(uint32_t id) {
    return m_geometryManager.acquire(id);
}

void ResourceManager::destroyGeometry(uint32_t id) { m_geometryManager.destroy(id); }

void ResourceManager::destroyGeometry(Geometry& geometry) {
    m_geometryManager.destroy(geometry);
}

Geometry* ResourceManager::getDefaultGeometry3D() {
    return m_geometryManager.getDefault3D();
}

Geometry* ResourceManager::getDefaultGeometry2D() {
    return m_geometryManager.getDefault2D();
}

GeometryProperties3D ResourceManager::generatePlaneGeometryProperties(
  PlaneProperties& props
) {
    return m_geometryManager.generatePlaneGeometryProperties(props);
}

}  // namespace sl
