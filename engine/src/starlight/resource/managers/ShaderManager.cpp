#include "ShaderManager.h"

#include "starlight/core/utils/Log.h"

namespace sl {

ShaderManager::Config ShaderManager::defaultConfig = {
    .maxShaderCount      = 512,
    .maxUniformCount     = 32,
    .maxGlobalTextures   = 32,
    .maxInstanceTextures = 32
};

ShaderManager::ShaderManager(
  const GPUMemoryProxy& resourceProxy, const ResourceLoader& resourceLoader,
  const Config& conf
) :
    m_resourceProxy(resourceProxy),
    m_resourceLoader(resourceLoader), m_conf(conf),
    m_shaders(m_conf.maxShaderCount) {}

Shader* ShaderManager::load(const std::string& name) {
    LOG_TRACE("Loading shader: {}", name);

    auto cfg = m_resourceLoader.loadShaderConfig(name);
    LOG_DEBUG("Loaded shader config: {}", cfg->toString());

    return nullptr;
}

}  // namespace sl
