#pragma once

#include <unordered_map>

#include <kc/core/Singleton.hpp>

#include "starlight/core/utils/FileSystem.h"
#include "starlight/core/memory/Memory.hpp"
#include "starlight/renderer/Material.h"

#include "starlight/renderer/gpu/GPUMemoryProxy.h"

namespace sl {

// DEFINE_SUB_ERROR(MaterialError, core::StarlightError);

class MaterialManager : public kc::core::Singleton<MaterialManager> {
   public:
    explicit MaterialManager(
        TextureManager& textureManager, const GPUMemoryProxy& resourceProxy,
        std::string_view materialsPath, FileSystem& fileSystem = fileSystem
    );

    ~MaterialManager();

    Material* load(const std::string& name);
    Material* acquire(const std::string& name);
    Material* getDefaultMaterial();

    void destroy(const std::string& name);
    void destroyAll();

   private:
    void createDefaultMaterial();

    TextureManager& m_textureManager;
    GPUMemoryProxy m_resourceProxy;

    std::string_view m_materialsPath;

    std::unordered_map<std::string, Material> m_materials;
    Material m_defaultMaterial;
    FileSystem& m_fileSystem;
};

}  // namespace sl