#pragma once

#include <optional>

#include <kc/core/Singleton.hpp>

#include "managers/TextureManager.h"
#include "managers/MaterialManager.h"
#include "managers/MeshManager.h"
#include "managers/ShaderManager.h"

#include "starlight/renderer/gpu/Shader.h"
#include "starlight/renderer/gpu/Mesh.h"
#include "starlight/renderer/Material.h"
#include "starlight/renderer/Model.h"
#include "starlight/renderer/Skybox.h"

#include "starlight/renderer/gpu/ResourcePools.h"

namespace sl {

class ResourceManager : public kc::core::Singleton<ResourceManager> {
public:
    explicit ResourceManager(ResourcePools& resourcePools);

    Shader* loadShader(const std::string& name);

    Texture* loadTexture(const std::string& name);
    Texture* loadCubeTexture(const std::string& name);

    Texture* acquireTexture(const std::string& name) const;
    Texture* acquireTexture(u64 id) const;

    void destroyTexture(const std::string& name);

    Material* loadMaterial(const MaterialConfig& config);
    Material* loadMaterial(const std::string& name);
    Material* acquireMaterial(const std::string& name);
    Material* getDefaultMaterial();

    UniqPtr<Skybox> loadSkybox(const std::string& path, Shader& skyboxShader);
    std::optional<Model> loadModel(const std::string& path);

    void destroyMaterial(const std::string& name);

    Mesh* loadMesh(const MeshConfig auto& props) {
        return m_meshManager.load(props);
    }
    Mesh* acquireMesh(uint32_t id);

    void destroyMesh(uint32_t id);

    Mesh* getDefaultMesh3D();
    Mesh* getDefaultMesh2D();

    template <typename Callback>
    requires Callable<Callback, void, std::string, const Texture*>
    void forEachTexture(Callback&& callback) {
        m_textureManager.forEach(std::forward<Callback>(callback));
    }

private:
    ResourcePools& m_resourcePools;

    TextureManager m_textureManager;
    ShaderManager m_shaderManager;
    MaterialManager m_materialManager;
    MeshManager m_meshManager;

    Mesh* m_skyboxMesh;
};

}  // namespace sl