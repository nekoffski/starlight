#include "MaterialManager.h"

#include <optional>

#include <kc/json/Utils.hpp>

#include "TextureManager.h"
#include "starlight/resource/resources/MaterialConfig.h"

namespace sl {

MaterialManager::MaterialManager(
  ShaderManager& shaderManager, TextureManager& textureManager,
  RendererProxy& rendererProxy, const ResourceLoader& resourceLoader
) :
    m_shaderManager(shaderManager),
    m_textureManager(textureManager), m_rendererProxy(rendererProxy),
    m_resourceLoader(resourceLoader) {}

MaterialManager::~MaterialManager() {
    m_defaultMaterial.releaseInstanceResources();
    destroyAll();
}

Material* MaterialManager::getDefaultMaterial() { return &m_defaultMaterial; }

void MaterialManager::createDefaultMaterial() {
    TextureMap diffuseMap(
      m_textureManager.getDefaultTexture(), Texture::Use::diffuseMap
    );

    m_defaultMaterial.generation   = 0;
    m_defaultMaterial.internalId   = 0;
    m_defaultMaterial.name         = "internal-default-material";
    m_defaultMaterial.diffuseColor = Vec4f{ 1.0f };
    m_defaultMaterial.diffuseMap   = diffuseMap;
    m_defaultMaterial.id           = invalidId;
    m_defaultMaterial.shader       = m_shaderManager.get("Builtin.Shader.Material");
    m_defaultMaterial.shininess    = 32.0f;
    m_defaultMaterial.acquireInstanceResources();
}

Material* MaterialManager::load(const std::string& name) {
    LOG_TRACE("Loading material '{}'", name);

    if (auto material = m_materials.find(name); material != m_materials.end()) {
        LOG_WARN(
          "Material '{}' already stored, returning pointer to the existing one", name
        );
        return &material->second;
    }

    LOG_TRACE("Found material file, will try to process");

    auto materialConfig = m_resourceLoader.loadMaterialConfig(name);

    if (not materialConfig.has_value()) {
        LOG_WARN("Could not process material file '{}'", name);
        return nullptr;
    }

    const auto getTexture = [&](const std::string& textureName) -> Texture* {
        if (auto texture = m_textureManager.acquire(textureName); texture) {
            LOG_DEBUG(
              "Found diffuse map '{}' required by material '{}'", texture->name, name
            );
            return texture;
        } else {
            LOG_INFO(
              "Diffuse map '{}' required by material '{}' not found, will try to load",
              textureName, name
            );
            return m_textureManager.load(textureName);
        }
        LOG_WARN("Could not find/load texture: {}, will use default", textureName);
        return m_textureManager.getDefaultTexture();
    };

    Material material;
    material.generation   = 0;
    material.internalId   = 0;
    material.name         = name;
    material.diffuseColor = materialConfig->diffuseColor;
    material.shininess    = materialConfig->shininess;

    material.diffuseMap = {
        getTexture(materialConfig->diffuseMap), Texture::Use::diffuseMap
    };
    material.specularMap = {
        getTexture(materialConfig->specularMap), Texture::Use::specularMap
    };

    material.id     = invalidId;
    material.shader = m_shaderManager.get(materialConfig->shaderName);

    ASSERT(
      material.shader, "Could not find shader: {} for material: {}",
      materialConfig->shaderName, name
    );

    material.acquireInstanceResources();
    m_materials[name] = std::move(material);

    return &m_materials[name];
}

Material* MaterialManager::acquire(const std::string& name) {
    LOG_TRACE("Acquiring material '{}'", name);
    if (auto material = m_materials.find(name); material != m_materials.end()) {
        return &material->second;
    } else {
        LOG_WARN("Material {} not found", name);
        return load(name);
    }
}

void MaterialManager::destroy(const std::string& name) {
    LOG_TRACE("Destroying material '{}'", name);
    // TODO: should we also destroy texture?
    if (auto material = m_materials.find(name); material != m_materials.end())
      [[likely]] {
        material->second.releaseInstanceResources();
        m_materials.erase(material);
    } else {
        LOG_WARN("Attempt to destroy not existing material - {}, will ignore", name);
    }
}

void MaterialManager::destroyAll() {
    for (auto& material : m_materials | std::views::values)
        material.releaseInstanceResources();
    m_materials.clear();
}

}  // namespace sl
