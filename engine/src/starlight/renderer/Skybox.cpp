#include "Skybox.h"

namespace sl {

Skybox::Skybox(TextureMap* cubeMap, Geometry* geometry, Shader& shader) :
    cubeMap(cubeMap), geometry(geometry),
    instanceId(shader.acquireInstanceResources({ cubeMap })), m_shader(shader) {}

Skybox::~Skybox() { m_shader.releaseInstanceResources(instanceId); }

}  // namespace sl
