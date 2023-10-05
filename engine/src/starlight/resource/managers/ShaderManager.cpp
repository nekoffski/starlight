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

    auto shader = findSlot();

    if (not shader) {
        LOG_ERROR(
          "Could not find free shader slot, consider changing reconfiguring maximum shader count"
        );
        return nullptr;
    }

    shader->state                  = Shader::State::notCreated;
    shader->name                   = cfg->name;
    shader->useInstances           = cfg->useInstances;
    shader->pushConstantRangeCount = 0;
    shader->attributeStride        = 0;
    shader->globalUboSize          = 0;
    shader->uboSize                = 0;
    shader->pushConstantStride     = 128;
    shader->pushConstantSize       = 0;
    shader->boundInstanceId.invalidate();

    // getRenderpassId from backend
    // getShaderImpl from backend

    shader->state = Shader::State::uninitialized;

    for (auto& attribute : cfg->attributes) addAttribute(shader, attribute);
    for (auto& uniform : cfg->uniforms) addUniform(shader, uniform);

    // initialize shader from backend

    // check if it wasn't inserted previously!
    m_shaderByName[name] = shader;

    return shader;
}

Shader* ShaderManager::findSlot() {
    for (int i = 0; i < m_conf.maxShaderCount; ++i) {
        if (auto shader = &m_shaders[i]; not shader->id) {
            shader->id = i;
            return shader;
        }
    }
    return nullptr;
}

void checkIfValidForAddingUniform(Shader* shader, const std::string& uniformName) {
    ASSERT(
      shader->state == Shader::State::uninitialized,
      "Uniforms can be added only in unitialized state"
    );
    ASSERT(
      not shader->uniforms.contains(uniformName),
      "Uniform already added, something went really wrong"
    );
}

void ShaderManager::addAttribute(Shader* shader, const ShaderAttributeConfig& cfg) {
    shader->attributeStride += cfg.size;
    shader->attributes.emplace_back(cfg.name, cfg.type, cfg.size);
}

void ShaderManager::addUniform(Shader* shader, const ShaderUniformConfig& cfg) {
    checkIfValidForAddingUniform(shader, cfg.name);

    if (cfg.type == ShaderUniform::Type::sampler)
        addSampler(shader, cfg);
    else
        addUniformImpl(shader, cfg.name, cfg.size, cfg.type, cfg.scope, 0, false);
}

void ShaderManager::addSampler(Shader* shader, const ShaderUniformConfig& cfg) {
    ASSERT(
      cfg.scope != ShaderScope::instance || shader->useInstances,
      "CAnnot ad instance sampler for shader that doesn't support instances"
    );
    ASSERT(
      cfg.scope != ShaderScope::local, "Samplers cannot be used at local scope"
    );

    u32 location = 0;

    if (cfg.scope == ShaderScope::global) {
        const auto globalTextureCount = shader->globalTextures.size();
        ASSERT(
          globalTextureCount + 1 <= m_conf.maxGlobalTextures,
          "Shader global texture count {} exceed maximum {}", globalTextureCount + 1,
          globalTextureCount
        );
        location = globalTextureCount;
    } else {
        ASSERT(
          shader->instanceTextureCount + 1 <= m_conf.maxInstanceTextures,
          "Shader instance texture count {} exceed maximum {}",
          shader->instanceTextureCount + 1, m_conf.maxInstanceTextures
        );
        location = shader->instanceTextureCount;
        ++shader->instanceTextureCount;
    }
    addUniformImpl(shader, cfg.name, 0, cfg.type, cfg.scope, location, true);
}

void ShaderManager::addUniformImpl(
  Shader* shader, const std::string& name, u32 size, ShaderUniform::Type type,
  ShaderScope scope, u32 setLocation, bool isSampler
) {
    const auto uniformCount = shader->uniforms.size();

    ASSERT(
      uniformCount + 1 <= m_conf.maxUniformCount,
      "Uniform count {} exceeds limit {}", uniformCount + 1, m_conf.maxUniformCount
    );

    ShaderUniform uniform;

    uniform.index    = uniformCount;
    uniform.scope    = scope;
    uniform.type     = type;
    uniform.location = isSampler ? setLocation : uniform.index;

    if (scope != ShaderScope::local) {
        uniform.setIndex = static_cast<u32>(scope);
        uniform.offset =
          isSampler ? 0
          : (scope == ShaderScope::global)
            ? shader->globalUboSize
            : shader->uboSize;
        uniform.size = isSampler ? 0 : size;
    } else {
        ASSERT(
          scope != ShaderScope::instance || shader->useInstances,
          "Cannot add a local uniform for shader that doesn't support locals"
        );

        uniform.setIndex.invalidate();

        // range
        // shader->pushConstantRanges[shader->pushConstantRangeCount++] = r;
    }

    shader->uniforms[name] = uniform;

    if (not isSampler) {
        if (uniform.scope == ShaderScope::global)
            shader->globalUboOffset += uniform.size;
        else
            shader->uboSize += uniform.size;
    }
}

}  // namespace sl
