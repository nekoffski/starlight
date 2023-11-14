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
    m_textureManager(textureManager), m_rendererProxy(rendererProxy) {
    static constexpr u32 maxMaterials = 1024;
    LOG_DEBUG("Max materials={}", maxMaterials);
    m_materials.resize(maxMaterials);
}

MaterialManager::~MaterialManager() { destroyAll(); }

Material* MaterialManager::getDefaultMaterial() { return m_defaultMaterial; }

void MaterialManager::createDefaultMaterial() {
    Material::Properties props;
    props.name         = "Internal.Material.Default";
    props.diffuseColor = Vec4f{ 1.0f };
    props.diffuseMap   = m_rendererProxy.createTextureMap(
        TextureMap::Properties{ .use = TextureMap::Use::diffuseMap },
        *Texture::defaultDiffuse
      );
    props.normalMap = m_rendererProxy.createTextureMap(
      TextureMap::Properties{ .use = TextureMap::Use::normalMap },
      *Texture::defaultNormal
    );
    props.specularMap = m_rendererProxy.createTextureMap(
      TextureMap::Properties{ .use = TextureMap::Use::specularMap },
      *Texture::defaultSpecular
    );
    props.shininess = 32.0f;

    m_defaultMaterial =
      create(props, *m_shaderManager.load("Builtin.Shader.Material"));
}

Material* MaterialManager::load(const std::string& name) {
    LOG_TRACE("Loading material '{}'", name);

    if (auto material = m_materialsLUT.find(name);
        material != m_materialsLUT.end()) {
        LOG_INFO(
          "Material '{}' already stored, returning pointer to the existing one", name
        );
        return material->second;
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

    props.diffuseMap = m_rendererProxy.createTextureMap(
      TextureMap::Properties{ .use = TextureMap::Use::diffuseMap },
      *getTexture(config.diffuseMap).value_or(Texture::defaultDiffuse)
    );
    props.normalMap = m_rendererProxy.createTextureMap(
      TextureMap::Properties{ .use = TextureMap::Use::normalMap },
      *getTexture(config.normalMap).value_or(Texture::defaultNormal)
    );
    props.specularMap = m_rendererProxy.createTextureMap(
      TextureMap::Properties{ .use = TextureMap::Use::specularMap },
      *getTexture(config.specularMap).value_or(Texture::defaultSpecular)
    );

    auto shader = m_shaderManager.load(config.shaderName);

    ASSERT(
      shader, "Could not find shader: {} for material: {}", config.shaderName,
      config.name
    );
    return create(props, *shader);
}

Material* MaterialManager::acquire(const std::string& name) {
    LOG_TRACE("Acquiring material '{}'", name);
    if (auto material = m_materialsLUT.find(name);
        material != m_materialsLUT.end()) {
        return material->second;
    } else {
        LOG_WARN("Material {} not found", name);
        return load(name);
    }
}

void MaterialManager::destroy(const std::string& name) {
    LOG_TRACE("Destroying material '{}'", name);
    // TODO: should we also destroy texture?
    if (auto material = m_materialsLUT.find(name); material != m_materialsLUT.end())
      [[likely]] {
        // TODO: I really don't like this idea, come up with RAII solution
        material->second->destroyTextureMaps(m_rendererProxy);
        auto id = material->second->getId();

        m_materials[id].clear();
        m_materialsLUT.erase(material);
    } else {
        LOG_WARN("Attempt to destroy not existing material - {}, will ignore", name);
    }
}

void MaterialManager::destroyAll() {
    m_materialsLUT.clear();
    m_materials.clear();
    m_defaultMaterial = nullptr;
}

Material* MaterialManager::create(
  const Material::Properties& props, Shader& shader
) {
    const auto id = findSlot();
    m_materials[id].emplace(props, id, shader);
    auto material              = m_materials[id].get();
    m_materialsLUT[props.name] = material;
    return material;
}

int MaterialManager::findSlot() const {
    LOG_DEBUG("Looking for free material slot");
    for (int i = 0; i < m_materials.size(); ++i) {
        if (not m_materials[i]) {
            LOG_DEBUG("Found slot: {}", i);
            return i;
        }
    }
    // TODO: it's not critical error..
    FAIL("Could not find free material slot, consider changing configuration");
}

}  // namespace sl
