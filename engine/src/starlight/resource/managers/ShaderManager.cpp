#include "ShaderManager.h"

#include "starlight/core/utils/Log.h"

namespace sl {

ShaderManager::ShaderManager(
  ResourcePools& resourcePools, const TextureManager& textureManager
) :
    m_resourcePools(resourcePools),
    m_textureManager(textureManager) {}

ShaderManager::~ShaderManager() { destroyAll(); }

Shader* ShaderManager::load(const std::string& name) {
    LOG_TRACE("Loading shader: {}", name);

    if (auto shader = acquire(name); shader) {
        LOG_INFO("Shader '{}' already loaded, returning cached one", name);
        return shader;
    }

    auto config = ShaderConfig::load(name, Texture::defaultDiffuse);
    auto shader = m_resourcePools.createShader(config->properties);

    if (not shader) {
        LOG_ERROR("Could not create shader '{}'", name);
        return nullptr;
    }

    m_shaders[name] = shader;
    return shader;
}

Shader* ShaderManager::acquire(const std::string& name) {
    if (auto shader = m_shaders.find(name); shader != m_shaders.end())
        return shader->second;
    LOG_WARN("Shader with name='{}' not found", name);
    return nullptr;
}

void ShaderManager::destroy(const std::string& name) {
    if (auto shader = m_shaders.find(name); shader == m_shaders.end()) {
        LOG_WARN("Trying to destroy shader '{}' that couldn't be found", name);
        return;
    } else {
        m_resourcePools.destroyShader(*shader->second);
    }
}

void ShaderManager::destroyAll() {
    LOG_TRACE("Destroying all shaders");
    for (auto& shader : m_shaders | std::views::values)
        m_resourcePools.destroyShader(*shader);
    m_shaders.clear();
}

}  // namespace sl
