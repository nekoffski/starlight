#include "Skybox.h"

namespace sl {

Skybox::Skybox(TextureMap* cubeMap, Mesh* mesh, Shader& shader) :
    cubeMap(cubeMap), mesh(mesh),
    instanceId(shader.acquireInstanceResources({ cubeMap })), m_shader(shader) {}

Skybox::~Skybox() { m_shader.releaseInstanceResources(instanceId); }

}  // namespace sl
