#include "MaterialManager.h"

#include <optional>

#include <kc/json/Utils.hpp>

#include "TextureManager.h"

namespace sl {

MaterialManager::MaterialManager(
  ShaderManager& shaderManager, TextureManager& textureManager,
  RendererProxy& rendererProxy
) :
    m_shaderManager(shaderManager),
    m_textureManager(textureManager), m_rendererProxy(rendererProxy) {}

MaterialManager::~MaterialManager() {
    m_defaultMaterial.releaseInstanceResources();
    destroyAll();
}

Material* MaterialManager::getDefaultMaterial() { return &m_defaultMaterial; }

void MaterialManager::createDefaultMaterial() {
    m_defaultMaterial.generation   = 0;
    m_defaultMaterial.internalId   = 0;
    m_defaultMaterial.name         = "Internal.Material.Default";
    m_defaultMaterial.diffuseColor = Vec4f{ 1.0f };
    m_defaultMaterial.diffuseMap   = {
          m_textureManager.getDefaultTexture(), Texture::Use::diffuseMap
    };
    // TODO: create default normal/specular maps
    m_defaultMaterial.normalMap = {
        m_textureManager.getDefaultNormalMap(), Texture::Use::normalMap
    };
    m_defaultMaterial.specularMap = {
        m_textureManager.getDefaultSpecularMap(), Texture::Use::specularMap
    };
    m_defaultMaterial.id        = invalidId;
    m_defaultMaterial.shader    = m_shaderManager.get("Builtin.Shader.Material");
    m_defaultMaterial.shininess = 32.0f;
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

    if (auto materialConfig = MaterialConfig::load(name); not materialConfig) {
        LOG_WARN("Could not process material file '{}'", name);
        return nullptr;
    } else {
        return load(*materialConfig);
    }
}

Material* MaterialManager::load(const MaterialConfig& config) {
    const auto getTexture =
      [&](const std::string& textureName) -> std::optional<Texture*> {
        if (auto texture = m_textureManager.acquire(textureName); texture) {
            LOG_DEBUG(
              "Found texture map '{}' required by material '{}'", texture->name,
              config.name
            );
            return texture;
        } else {
            LOG_INFO(
              "Texture map '{}' required by material '{}' not found, will try to load",
              textureName, config.name
            );
            return m_textureManager.load(textureName);
        }
        LOG_WARN("Could not find/load texture: {}, returning nullptr", textureName);
        return {};
    };

    Material material;
    material.generation   = 0;
    material.internalId   = 0;
    material.name         = config.name;
    material.diffuseColor = config.diffuseColor;
    material.shininess    = config.shininess;

    material.diffuseMap = {
        getTexture(config.diffuseMap).value_or(m_textureManager.getDefaultTexture()),
        Texture::Use::diffuseMap
    };
    material.specularMap = {
        getTexture(config.specularMap)
          .value_or(m_textureManager.getDefaultSpecularMap()),
        Texture::Use::specularMap
    };
    material.normalMap = {
        getTexture(config.normalMap)
          .value_or(m_textureManager.getDefaultNormalMap()),
        Texture::Use::normalMap
    };

    material.id     = invalidId;
    material.shader = m_shaderManager.get(config.shaderName);

    ASSERT(
      material.shader, "Could not find shader: {} for material: {}",
      config.shaderName, config.name
    );

    material.acquireInstanceResources();
    m_materials[config.name] = std::move(material);

    return &m_materials[config.name];
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
