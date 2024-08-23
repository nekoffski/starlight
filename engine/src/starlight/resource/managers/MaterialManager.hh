#pragma once

#include <unordered_map>
#include <vector>

#include "starlight/core/ResourcePool.hh"
#include "starlight/core/FileSystem.hh"
#include "starlight/core/memory/Memory.hh"
#include "starlight/renderer/Material.hh"
#include "starlight/renderer/gpu/ResourcePools.hh"

#include "starlight/resource/configs/MaterialConfig.hh"

#include "ResourceManager.hh"
#include "ShaderManager.hh"

namespace sl {

class MaterialManager : public ResourceManager<Material, MaterialManager> {
    static constexpr u64 maxMaterials = 4096;

public:
    explicit MaterialManager(
      ShaderManager& shaderManager, TextureManager& textureManager,
      ResourcePools& resourcePools
    );
    ~MaterialManager();

    Material* load(const std::string& name);
    Material* load(const MaterialConfig& config);

    Material* getDefaultMaterial();
    void createDefaultMaterial();

private:
    void destroyInternals(Material* resource) override;

    ShaderManager& m_shaderManager;
    TextureManager& m_textureManager;
    ResourcePools& m_resourcePools;

    ResourcePool<Material> m_materials;
    Material* m_defaultMaterial;
};

}  // namespace sl