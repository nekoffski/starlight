#include "ShaderManager.h"

#include "starlight/core/utils/Log.h"

#include "starlight/resource/resources/ShaderConfig.h"

namespace sl {

ShaderManager::ShaderManager(
  ResourcePools& resourcePools, TextureManager& textureManager
) :
    m_resourcePools(resourcePools),
    m_textureManager(textureManager) {}

ShaderManager::~ShaderManager() {
    forEach([&]([[maybe_unused]] u64, Shader* shader) {
        m_resourcePools.destroyShader(*shader);
    });
}

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
    return storeResource(name, shader->getId(), shader);
}

void ShaderManager::destroyInternals(Shader* shader) {
    m_resourcePools.destroyShader(*shader);
}

std::string ShaderManager::getResourceName() const { return "Shader"; }

}  // namespace sl
