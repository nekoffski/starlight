#pragma once

#include <unordered_map>

#include <kc/core/Singleton.hpp>

#include "starlight/core/FileSystem.h"
#include "starlight/core/Memory.hpp"
#include "starlight/core/Core.hpp"

#include "Material.h"
#include "ResourceProxy.h"

#include "fwd.h"

namespace sl::gfx {

DEFINE_SUB_ERROR(MaterialError, core::NovaError);

class MaterialManager : public kc::core::Singleton<MaterialManager> {
   public:
    explicit MaterialManager(
        TextureManager& textureManager, const ResourceProxy& resourceProxy,
        std::string_view materialsPath, core::FileSystem& fileSystem = core::fileSystem
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
    ResourceProxy m_resourceProxy;

    std::string_view m_materialsPath;

    std::unordered_map<std::string, Material> m_materials;
    Material m_defaultMaterial;
    core::FileSystem& m_fileSystem;
};

}  // namespace sl::gfx