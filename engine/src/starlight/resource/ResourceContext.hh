
#pragma once

#include "All.hh"

namespace sl {

class ResourceContext {
public:
    explicit ResourceContext(ResourcePools& resourcePools);

private:
    ResourcePools& m_resourcePools;

    TextureManager m_textureManager;
    ShaderManager m_shaderManager;
    MaterialManager m_materialManager;
    MeshManager m_meshManager;
    SkyboxManager m_skyboxManager;
    ModelManager m_modelManager;
};

}  // namespace sl
