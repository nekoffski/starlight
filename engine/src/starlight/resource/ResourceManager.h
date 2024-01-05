#pragma once

#include <optional>

#include <kc/core/Singleton.hpp>

#include "managers/TextureManager.h"
#include "managers/MaterialManager.h"
#include "managers/GeometryManager.h"
#include "managers/ShaderManager.h"

#include "starlight/renderer/gpu/Shader.h"
#include "starlight/renderer/gpu/Geometry.h"
#include "starlight/renderer/Material.h"
#include "starlight/renderer/Mesh.h"

#include "starlight/renderer/gpu/ResourcePools.h"

namespace sl {

class ResourceManager : public kc::core::Singleton<ResourceManager> {
public:
    explicit ResourceManager(ResourcePools& resourcePools);

    Shader* loadShader(const std::string& name);

    Texture* loadTexture(const std::string& name);
    Texture* acquireTexture(const std::string& name) const;

    void destroyTexture(const std::string& name);

    Material* loadMaterial(const MaterialConfig& config);
    Material* loadMaterial(const std::string& name);
    Material* acquireMaterial(const std::string& name);
    Material* getDefaultMaterial();

    std::optional<Mesh> loadMesh(const std::string& path);

    void destroyMaterial(const std::string& name);

    Geometry* loadGeometry(GeometryConfig auto& props) {
        return m_geometryManager.load(props);
    }
    Geometry* acquireGeometry(uint32_t id);

    void destroyGeometry(uint32_t id);

    Geometry* getDefaultGeometry3D();
    Geometry* getDefaultGeometry2D();

    Shader* getUIDefaultShader() { return m_uiDefaultShader; }
    Shader* getMaterialDefaultShader() { return m_materialDefaultShader; }

private:
    TextureManager m_textureManager;
    ShaderManager m_shaderManager;
    MaterialManager m_materialManager;
    GeometryManager m_geometryManager;

    Shader* m_materialDefaultShader;
    Shader* m_uiDefaultShader;
};

}  // namespace sl