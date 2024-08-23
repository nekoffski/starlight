#include "Material.hh"

#include "starlight/core/Json.hh"

#include "gpu/ResourcePools.hh"

namespace sl {

Material::Material(u64 id, const Properties& props, ResourceRef<Shader> shader) :
    m_id(id), m_props(props), m_shader(shader), m_renderFrameNumber(0) {
    LOG_TRACE("Creating Material: {}", m_props.name);
    m_instanceId = shader->acquireInstanceResources(
      { m_props.diffuseMap.get(), m_props.specularMap.get(),
        m_props.normalMap.get() }
    );
    LOG_DEBUG("Material '{}' instance id: {}", m_props.name, m_instanceId);
}

Material::~Material() {
    LOG_TRACE("Destroying Material");
    m_shader->releaseInstanceResources(m_instanceId);
}

bool Material::isTransparent() const {
    return m_props.diffuseMap->getProperties().isTransparent;
}

void Material::applyUniforms(u64 renderFrameNumber) {
    if (m_renderFrameNumber != renderFrameNumber) {
        m_shader->setInstanceUniforms(
          m_instanceId,
          [&](Shader::UniformProxy& proxy) {
              proxy.set("diffuseColor", m_props.diffuseColor);
              proxy.set("diffuseTexture", m_props.diffuseMap);
              proxy.set("specularTexture", m_props.specularMap);
              proxy.set("normalTexture", m_props.normalMap);
              proxy.set("shininess", m_props.shininess);
          }
        );
        m_renderFrameNumber = renderFrameNumber;
    }
}

u64 Material::getId() const { return m_id; }

const std::string& Material::getName() const { return m_props.name; }

const Material::Properties& Material::getProperties() const { return m_props; }

ResourceRef<Material> Material::load(
  RendererBackend& renderer, const std::string& name, std::string_view materialsPath,
  const FileSystem& fs
) {
    return MaterialManager::get().load(renderer, name, materialsPath, fs);
}

ResourceRef<Material> Material::find(const std::string& name) {
    return MaterialManager::get().find(name);
}

ResourceRef<Material> MaterialManager::load(
  RendererBackend& renderer, const std::string& name, std::string_view materialsPath,
  const FileSystem& fs
) {
    if (auto config = Material::Config::load(name, materialsPath, fs); not config) {
        LOG_WARN("Could not load material config for '{}'", name);
        return nullptr;
    } else {
        Material::Properties properties;

        properties.name         = config->name;
        properties.diffuseColor = config->diffuseColor;
        properties.shininess    = config->shininess;
        properties.diffuseMap =
          Texture::load(config->diffuseMap, Texture::Type::flat);
        properties.normalMap = Texture::load(config->normalMap, Texture::Type::flat);
        properties.specularMap =
          Texture::load(config->specularMap, Texture::Type::flat);

        auto shader = Shader::load(config->shaderName);
        return store(name, createOwningPtr<Material>(1u, properties, shader));
    }
}

Material::Config Material::Config::createDefault(const std::string& name) {
    return Material::Config{
        .name         = name,
        .diffuseColor = defaultDiffuseColor,
        .shininess    = defaultShininess,
        .diffuseMap   = defaultDiffuseMap,
        .specularMap  = defaultSpecularMap,
        .normalMap    = defaultNormalMap,
        .shaderName   = defaultShader
    };
}

std::optional<Material::Config> Material::Config::load(
  const std::string& name, std::string_view materialsPath, const FileSystem& fs
) {
    const auto fullPath = fmt::format("{}/{}.json", materialsPath, name);

    LOG_TRACE("Loading material config file: {}", fullPath);

    if (not fs.isFile(fullPath)) {
        LOG_ERROR("Could not find file: '{}'", fullPath);
        return {};
    }

    try {
        const auto root = kc::json::loadJson(fs.readFile(fullPath));

        return Material::Config{
            .name         = name,
            .diffuseColor = getFieldOr(root, "diffuse-color", defaultDiffuseColor),
            .shininess    = getFieldOr(root, "shininess", defaultShininess),
            .diffuseMap   = getFieldOr(root, "diffuse-map", defaultDiffuseMap),
            .specularMap  = getFieldOr(root, "specular-map", defaultSpecularMap),
            .normalMap    = getFieldOr(root, "normal-map", defaultNormalMap),
            .shaderName   = getFieldOr(root, "shader-name", defaultShader)
        };
    } catch (kc::json::JsonError& e) {
        LOG_ERROR("Could not parse material '{}' file: {}", name, e.asString());
    }
    return {};
}

}  // namespace sl
