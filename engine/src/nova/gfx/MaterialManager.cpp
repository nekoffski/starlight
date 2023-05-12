#include "MaterialManager.h"

#include <optional>

#include <kc/json/Utils.hpp>

#include "TextureManager.h"

#include "nova/res/MaterialConfig.h"

namespace nova::gfx {

MaterialManager::MaterialManager(
    TextureManager& textureManager, const ResourceProxy& resourceProxy,
    std::string_view materialsPath, core::FileSystem& fileSystem
)
    : m_textureManager(textureManager)
    , m_resourceProxy(resourceProxy)
    , m_materialsPath(materialsPath)
    , m_fileSystem(fileSystem) {
    createDefaultMaterial();
}

MaterialManager::~MaterialManager() {
    m_resourceProxy.releaseMaterialResources(m_defaultMaterial);
    destroyAll();
}

Material* MaterialManager::getDefaultMaterial() { return &m_defaultMaterial; }

void MaterialManager::createDefaultMaterial() {
    TextureMap diffuseMap(m_textureManager.getDefaultTexture(), Texture::Use::diffuseMap);

    m_defaultMaterial.generation   = 0;
    m_defaultMaterial.internalId   = 0;
    m_defaultMaterial.name         = "internal-default-material";
    m_defaultMaterial.diffuseColor = math::Vec4f{1.0f};
    m_defaultMaterial.diffuseMap   = diffuseMap;
    m_defaultMaterial.id           = core::invalidId;

    // TODO: RAII?
    m_resourceProxy.acquireMaterialResources(m_defaultMaterial);
}

Material* MaterialManager::load(const std::string& name) {
    LOG_TRACE("Loading material '{}'", name);
    static const std::string_view extension = "nvmat";

    if (auto material = m_materials.find(name); material != m_materials.end()) {
        LOG_WARN("Material '{}' already stored, returning pointer to the existing one", name);
        return &material->second;
    }

    const auto fullPath = fmt::format("{}/{}.{}", m_materialsPath, name, extension);

    if (not m_fileSystem.isFile(fullPath)) {
        LOG_WARN("Could not find material file '{}'", fullPath);
        return nullptr;
    }

    LOG_TRACE("Found material file, will try to process");

    auto materialConfig = res::MaterialConfig::create(name);

    if (not materialConfig.has_value()) {
        LOG_WARN("Could not process material file '{}'", fullPath);
        return nullptr;
    }

    const auto getDiffuseMapTexture = [&](const std::string& diffuseMapName) -> gfx::Texture* {
        if (auto texture = m_textureManager.acquire(diffuseMapName); texture) {
            LOG_DEBUG("Found diffuse map '{}' required by material '{}'", texture->name, name);
            return texture;
        } else {
            LOG_INFO(
                "Diffuse map '{}' required by material '{}' not found, will try to load",
                diffuseMapName, name
            );
            return m_textureManager.load(diffuseMapName);
        }
    };

    TextureMap diffuseMap(
        getDiffuseMapTexture(materialConfig->diffuseMap), Texture::Use::diffuseMap
    );

    Material material;
    material.generation   = 0;
    material.internalId   = 0;
    material.name         = name;
    material.diffuseColor = materialConfig->diffuseColor;
    material.diffuseMap   = diffuseMap;
    material.id           = core::invalidId;

    m_resourceProxy.acquireMaterialResources(material);

    m_materials[name] = std::move(material);

    return &m_materials[name];
}

Material* MaterialManager::acquire(const std::string& name) {
    LOG_TRACE("Acquiring material '{}'", name);
    if (auto material = m_materials.find(name); material != m_materials.end()) {
        return &material->second;
    } else {
        LOG_WARN("Material {} not found", name);
        return nullptr;
    }
}

void MaterialManager::destroy(const std::string& name) {
    LOG_TRACE("Destroying material '{}'", name);
    // TODO: should we also destroy texture?
    if (auto material = m_materials.find(name); material != m_materials.end()) [[likely]] {
        m_resourceProxy.releaseMaterialResources(material->second);
        m_materials.erase(material);
    } else {
        LOG_WARN("Attempt to destroy not existing material - {}, will ignore", name);
    }
}

void MaterialManager::destroyAll() {
    for (auto& material : m_materials | std::views::values)
        m_resourceProxy.releaseMaterialResources(material);
    m_materials.clear();
}

}  // namespace nova::gfx
