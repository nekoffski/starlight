#include "MaterialManager.h"

#include <optional>

#include <kc/json/Utils.hpp>

#include "TextureManager.h"

namespace nova::gfx {

struct MaterialConfig {
    static std::optional<MaterialConfig> load(const std::string& content, const std::string& name);

    math::Vec4f diffuseColor;
    std::string diffuseMap;
};

MaterialManager::MaterialManager(
    TextureManager* textureManager, std::string_view materialsPath, core::FileSystem* fileSystem
)
    : m_textureManager(textureManager)
    , m_materialsPath(m_materialsPath)
    , m_fileSystem(fileSystem) {}

Material* MaterialManager::load(const std::string& name) {
    LOG_TRACE("Loading material '{}'", name);
    static const std::string_view extension = "nvmat";

    ASSERT(not m_materials.contains(name), "Material '{}' already stored", name);
    const auto fullPath = fmt::format("{}/{}.{}", m_materialsPath, name, extension);

    ASSERT(m_fileSystem->isFile(fullPath), "Could not find material file '{}'", fullPath);

    auto materialConfig = MaterialConfig::load(m_fileSystem->readFile(fullPath), name);
    ASSERT(materialConfig.has_value(), "Could not process material file '{}'", fullPath);

    const auto getDiffuseMapTexture = [&](const std::string& diffuseMapName) -> gfx::Texture* {
        if (auto texture = m_textureManager->acquire(diffuseMapName); texture) {
            LOG_DEBUG("Found diffuse map '{}' required by material '{}'", texture->name, name);
            return texture;
        } else {
            LOG_INFO(
                "Diffuse map '{}' required by material '{}' not found, will try to load",
                diffuseMapName, name
            );
            return m_textureManager->load(diffuseMapName);
        }
    };

    TextureMap diffuseMap(
        getDiffuseMapTexture(materialConfig->diffuseMap), Texture::Use::diffuseMap
    );

    Material material;
    material.diffuseColor = materialConfig->diffuseColor;
    material.internalId   = 0;
    material.name         = name;
    material.diffuseColor = materialConfig->diffuseColor;
    material.diffuseMap   = diffuseMap;

    m_materials[name] = std::move(material);

    return &m_materials[name];
}

// TODO: it should return nullptr or throw an error in case of not existing material
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
    if (auto material = m_materials.find(name); material != m_materials.end()) [[likely]]
        m_materials.erase(material);
    else
        LOG_WARN("Attempt to destroy not existing material - {}, will ignore", name);
}

void MaterialManager::destroyAll() { m_materials.clear(); }

auto fieldFrom(auto& root) { return kc::json::fieldFrom<kc::json::JsonError>(root); }

std::optional<MaterialConfig> MaterialConfig::load(
    const std::string& content, const std::string& name
) {
    MaterialConfig config;
    try {
        auto root = kc::json::loadJson(content);

        config.diffuseColor = fieldFrom(root).withName("diffuse-color").ofType<math::Vec4f>().get();
        config.diffuseMap   = fieldFrom(root).withName("diffuse-map").ofType<std::string>().get();

        return config;
    } catch (kc::json::JsonError& e) {
        LOG_ERROR("Could not parse material '{}' file: {}", name, e.what());
        return {};
    }
}

}  // namespace nova::gfx
