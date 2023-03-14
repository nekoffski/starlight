#pragma once

#include <unordered_map>

#include <kc/core/Singleton.hpp>

#include "nova/core/FileSystem.h"
#include "nova/core/Memory.hpp"
#include "nova/core/Core.hpp"

#include "Material.h"
#include "ResourceProxy.h"

#include "fwd.h"

namespace nova::gfx {

DEFINE_SUB_ERROR(MaterialError, core::NovaError);

class MaterialManager : public kc::core::Singleton<MaterialManager> {
   public:
    explicit MaterialManager(
        TextureManager& textureManager, const ResourceProxy& resourceProxy,
        std::string_view materialsPath, core::FileSystem& fileSystem = core::fileSystem
    );

    Material* load(const std::string& name);
    Material* acquire(const std::string& name);

    void destroy(const std::string& name);
    void destroyAll();

   private:
    TextureManager& m_textureManager;
    ResourceProxy m_resourceProxy;

    std::string_view m_materialsPath;

    std::unordered_map<std::string, Material> m_materials;
    core::FileSystem& m_fileSystem;
};

}  // namespace nova::gfx