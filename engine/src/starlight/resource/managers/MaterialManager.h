#pragma once

#include <unordered_map>
#include <vector>

#include "starlight/core/utils/ResourcePool.hpp"
#include "starlight/core/utils/FileSystem.h"
#include "starlight/core/memory/Memory.hpp"
#include "starlight/renderer/Material.h"
#include "starlight/renderer/gpu/ResourcePools.h"

#include "starlight/resource/resources/MaterialConfig.h"

#include "ResourceManager.hpp"
#include "ShaderManager.h"

namespace sl {

class MaterialManager : public ResourceManager<Material, MaterialManager> {
    static constexpr u64 maxMaterials = 4096;

public:
    explicit MaterialManager(
      ShaderManager& shaderManager, TextureManager& textureManager,
      ResourcePools& resourcePools
    );
    ~MaterialManager() override;

    Material* load(const std::string& name);
    Material* load(const MaterialConfig& config);

    Material* getDefaultMaterial();
    void createDefaultMaterial();

private:
    void destroyInternals(Material* resource) override;
    std::string getResourceName() const override;

    ShaderManager& m_shaderManager;
    TextureManager& m_textureManager;
    ResourcePools& m_resourcePools;

    ResourcePool<Material> m_materials;
    Material* m_defaultMaterial;
};

}  // namespace sl