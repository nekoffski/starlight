#include "Skybox.h"

namespace sl {

Skybox::Skybox(u64 id, Texture& cubeMap, Mesh& mesh, Shader& shader) :
    m_cubeMap(cubeMap), m_mesh(mesh),
    m_instanceId(shader.acquireInstanceResources({ &cubeMap })), m_shader(shader) {}

ResourceRef<Skybox> Skybox::load(const std::string& name) {
    return ResourceRef<Skybox>();
}

Skybox::~Skybox() { m_shader.releaseInstanceResources(m_instanceId); }

u64 Skybox::getId() const { return m_id; }

Mesh* Skybox::getMesh() { return &m_mesh; }

u32 Skybox::getInstanceId() const { return m_instanceId; }

Texture* Skybox::getCubeMap() { return &m_cubeMap; }

SkyboxManager::SkyboxManager(RendererBackend& renderer) : m_renderer(renderer) {}

ResourceRef<Skybox> SkyboxManager::load(const std::string& name) {
    return ResourceRef<Skybox>();
}

}  // namespace sl
