#include "Material.h"

#include "Shader.h"
#include "ResourcePools.h"

namespace sl {

Material::Material(const Properties& props, u32 id, Shader& shader) :
    m_props(props), m_id(id), m_shader(shader), m_renderFrameNumber(0) {
    LOG_TRACE("Creating Material: {}", m_props.name);
    m_instanceId = shader.acquireInstanceResources(
      { m_props.diffuseMap, m_props.specularMap, m_props.normalMap }
    );
    LOG_DEBUG("Material '{}' instance id: {}", m_props.name, m_instanceId);
}

Material::~Material() {
    LOG_TRACE("Destroying Material");
    m_shader.releaseInstanceResources(m_instanceId);
}

void Material::applyUniforms(u32 renderFrameNumber) {
    if (m_renderFrameNumber != renderFrameNumber) {
        m_shader.setInstanceUniforms(m_instanceId, [&](Shader::UniformProxy& proxy) {
            proxy.set("diffuseColor", m_props.diffuseColor);
            proxy.set("diffuseTexture", m_props.diffuseMap);
            proxy.set("specularTexture", m_props.specularMap);
            proxy.set("normalTexture", m_props.normalMap);
            proxy.set("shininess", m_props.shininess);
        });
        m_renderFrameNumber = renderFrameNumber;
    }
}

u32 Material::getId() const { return m_id; }

const std::string& Material::getName() const { return m_props.name; }

void Material::destroyTextureMaps(ResourcePools& resourcePools) {
    resourcePools.destroyTextureMap(*m_props.diffuseMap);
    resourcePools.destroyTextureMap(*m_props.specularMap);
    resourcePools.destroyTextureMap(*m_props.normalMap);
}

}  // namespace sl
