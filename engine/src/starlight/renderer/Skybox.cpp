#include "Skybox.h"

namespace sl {

Skybox::Skybox(u64 id, TextureMap& cubeMap, Mesh& mesh, Shader& shader) :
    m_cubeMap(cubeMap), m_mesh(mesh),
    m_instanceId(shader.acquireInstanceResources({ &cubeMap })), m_shader(shader) {}

Skybox::~Skybox() { m_shader.releaseInstanceResources(m_instanceId); }

u64 Skybox::getId() const { return m_id; }

Mesh* Skybox::getMesh() { return &m_mesh; }

u32 Skybox::getInstanceId() const { return m_instanceId; }

TextureMap* Skybox::getCubeMap() { return &m_cubeMap; }

}  // namespace sl
