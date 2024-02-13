#include "ResourceContext.h"

namespace sl {

ResourceContext::ResourceContext(ResourcePools& resourcePools) :
    m_resourcePools(resourcePools), m_textureManager(resourcePools),
    m_shaderManager(resourcePools, m_textureManager),
    m_materialManager(m_shaderManager, m_textureManager, resourcePools),
    m_meshManager(resourcePools, m_materialManager) {
    m_materialManager.createDefaultMaterial();
}

}  // namespace sl
