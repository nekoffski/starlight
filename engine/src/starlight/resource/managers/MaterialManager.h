#pragma once

#include <unordered_map>
#include <vector>

#include "starlight/core/utils/FileSystem.h"
#include "starlight/core/memory/Memory.hpp"
#include "starlight/renderer/Material.h"
#include "starlight/renderer/gpu/ResourcePools.h"

#include "starlight/resource/resources/MaterialConfig.h"

#include "ShaderManager.h"

namespace sl {

class MaterialManager {
public:
    explicit MaterialManager(
      ShaderManager& shaderManager, TextureManager& textureManager,
      ResourcePools& resourcePools
    );

    ~MaterialManager();

    Material* load(const std::string& name);
    Material* load(const MaterialConfig& config);

    Material* acquire(const std::string& name);
    Material* getDefaultMaterial();

    void createDefaultMaterial();

    void destroy(const std::string& name);
    void destroyAll();

private:
    Material* create(const Material::Properties& props, Shader& shader);

    int findSlot() const;

    ShaderManager& m_shaderManager;
    TextureManager& m_textureManager;
    ResourcePools& m_resourcePools;

    std::vector<LocalPtr<Material>> m_materials;
    std::unordered_map<std::string, Material*> m_materialsLUT;

    Material* m_defaultMaterial;
};

}  // namespace sl