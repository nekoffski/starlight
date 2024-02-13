#include "MaterialManager.h"

#include <optional>

#include <kc/json/Utils.hpp>

#include "TextureManager.h"

namespace sl {

MaterialManager::MaterialManager(
  ShaderManager& shaderManager, TextureManager& textureManager,
  ResourcePools& resourcePools
) :
    m_shaderManager(shaderManager),
    m_textureManager(textureManager), m_resourcePools(resourcePools),
    m_materials("Material", maxMaterials) {}

MaterialManager::~MaterialManager() {
    forEach([&](u64 id, [[maybe_unused]] Material*) { m_materials.destroy(id); });
}

Material* MaterialManager::getDefaultMaterial() { return m_defaultMaterial; }

void MaterialManager::createDefaultMaterial() {
    Material::Properties props;
    props.name         = "Internal.Material.Default";
    props.diffuseColor = Vec4f{ 1.0f };
    props.diffuseMap   = m_resourcePools.createTextureMap(
        TextureMap::Properties{ .use = TextureMap::Use::diffuseMap },
        *Texture::defaultDiffuse
      );
    props.normalMap = m_resourcePools.createTextureMap(
      TextureMap::Properties{ .use = TextureMap::Use::normalMap },
      *Texture::defaultNormal
    );
    props.specularMap = m_resourcePools.createTextureMap(
      TextureMap::Properties{ .use = TextureMap::Use::specularMap },
      *Texture::defaultSpecular
    );
    props.shininess = 32.0f;

    auto shader   = m_shaderManager.load("Builtin.Shader.Material");
    auto material = m_materials.create(props, *shader);
    m_defaultMaterial =
      storeResource(material->getName(), material->getId(), material);
}

Material* MaterialManager::load(const std::string& name) {
    LOG_TRACE("Loading material '{}'", name);

    if (auto material = acquire(name); material) {
        LOG_INFO(
          "Material '{}' already stored, returning pointer to the existing one", name
        );
        return material;
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
              "Found texture map '{}' required by material '{}'",
              texture->getProperties().name, config.name
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

    Material::Properties props;
    props.name         = config.name;
    props.diffuseColor = config.diffuseColor;
    props.shininess    = config.shininess;

    props.diffuseMap = m_resourcePools.createTextureMap(
      TextureMap::Properties{ .use = TextureMap::Use::diffuseMap },
      *getTexture(config.diffuseMap).value_or(Texture::defaultDiffuse)
    );
    props.normalMap = m_resourcePools.createTextureMap(
      TextureMap::Properties{ .use = TextureMap::Use::normalMap },
      *getTexture(config.normalMap).value_or(Texture::defaultNormal)
    );
    props.specularMap = m_resourcePools.createTextureMap(
      TextureMap::Properties{ .use = TextureMap::Use::specularMap },
      *getTexture(config.specularMap).value_or(Texture::defaultSpecular)
    );

    auto shader = m_shaderManager.load(config.shaderName);

    ASSERT(
      shader, "Could not find shader: {} for material: {}", config.shaderName,
      config.name
    );
    auto material = m_materials.create(props, *shader);
    return storeResource(props.name, material->getId(), material);
}

void MaterialManager::destroyInternals(Material* material) {
    material->destroyTextureMaps(m_resourcePools);
}

std::string MaterialManager::getResourceName() const { return "Material"; }

}  // namespace sl
