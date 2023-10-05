#pragma once

#include <kc/core/Singleton.hpp>

#include "managers/TextureManager.h"
#include "managers/MaterialManager.h"
#include "managers/GeometryManager.h"
#include "managers/ShaderManager.h"

#include "starlight/renderer/Shader.h"
#include "starlight/renderer/Geometry.h"
#include "starlight/renderer/Material.h"

#include "starlight/renderer/gpu/GPUMemoryProxy.h"
#include "starlight/renderer/gpu/TextureLoader.h"
#include "ResourceLoader.h"

namespace sl {

class ResourceManager : public kc::core::Singleton<ResourceManager> {
public:
    explicit ResourceManager(
      TextureLoader& textureLoader, const GPUMemoryProxy& gpuMemoryProxy
    );

    Shader* loadShader(const std::string& name);

    Texture* loadTexture(const std::string& name);
    Texture* acquireTexture(const std::string& name) const;
    Texture* getDefaultTexture() const;

    void destroyTexture(const std::string& name);

    Material* loadMaterial(const std::string& name);
    Material* acquireMaterial(const std::string& name);
    Material* getDefaultMaterial();

    void destroyMaterial(const std::string& name);

    Geometry* loadGeometry(GeometryProperties auto& props) {
        return m_geometryManager.load(props);
    }
    Geometry* acquireGeometry(uint32_t id);

    void destroyGeometry(uint32_t id);
    void destroyGeometry(Geometry& geometry);

    Geometry* getDefaultGeometry3D();
    Geometry* getDefaultGeometry2D();

    GeometryProperties3D generatePlaneGeometryProperties(PlaneProperties& props);

private:
    ResourceLoader m_resourceLoader;

    TextureManager m_textureManager;
    MaterialManager m_materialManager;
    GeometryManager m_geometryManager;
    ShaderManager m_shaderManager;
};

}  // namespace sl