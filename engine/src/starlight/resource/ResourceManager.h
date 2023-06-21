#pragma once

#include "TextureManager.h"
#include "MaterialManager.h"
#include "GeometryManager.h"

#include "starlight/renderer/gpu/GPUMemoryProxy.h"
#include "starlight/renderer/gpu/TextureLoader.h"

namespace sl {

class ResourceManager {
   public:
    explicit ResourceManager(TextureLoader& textureLoader, const GPUMemoryProxy& gpuMemoryProxy)
        : m_textureManager(textureLoader, SL_ASSETS_TEXTURES_PATH)
        , m_materialManager(m_textureManager, gpuMemoryProxy, SL_ASSETS_MATERIALS_PATH)
        , m_geometryManager(gpuMemoryProxy) {}

    Texture* loadTexture(const std::string& name) { return m_textureManager.load(name); }

    Texture* acquireTexture(const std::string& name) const {
        return m_textureManager.acquire(name);
    }

    Texture* getDefaultTexture() const { return m_textureManager.getDefaultTexture(); }

    void destroyTexture(const std::string& name) { m_textureManager.destroy(name); }

    Material* loadMaterial(const std::string& name) { return m_materialManager.load(name); }

    Material* acquireMaterial(const std::string& name) { return m_materialManager.acquire(name); }

    Material* getDefaultMaterial() { return m_materialManager.getDefaultMaterial(); }

    void destroyMaterial(const std::string& name) { m_materialManager.destroy(name); }

    Geometry* acquireGeometry(uint32_t id) { return m_geometryManager.acquire(id); }

    Geometry* loadGeometry(GeometryProperties& props) { return m_geometryManager.load(props); }

    void destroyGeometry(uint32_t id) { m_geometryManager.destroy(id); }

    void destroyGeometry(Geometry& geometry) { m_geometryManager.destroy(geometry); }

    Geometry* getDefaultGeometry() { return m_geometryManager.getDefault(); }

    GeometryProperties generalePlaneGeometryProperties(PlaneProperties& props) {
        return m_geometryManager.generalePlaneGeometryProperties(props);
    }

   private:
    TextureManager m_textureManager;
    MaterialManager m_materialManager;
    GeometryManager m_geometryManager;
};

}  // namespace sl