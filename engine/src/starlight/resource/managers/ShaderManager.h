#pragma once

#include <kc/core/Singleton.hpp>

#include "starlight/renderer/ShaderScope.h"
#include "starlight/renderer/ShaderAttribute.h"
#include "starlight/renderer/ShaderUniform.h"
#include "starlight/renderer/Shader.h"
#include "starlight/renderer/gpu/RendererProxy.h"

#include "starlight/resource/ResourceLoader.h"

#include <vector>

namespace sl {

class ShaderManager {
public:
    struct Config {
        u64 maxShaderCount;
        u8 maxUniformCount;
        u8 maxGlobalTextures;
        u8 maxInstanceTextures;
    };

    static Config defaultConfig;

    explicit ShaderManager(
      RendererProxy& resourceProxy, const ResourceLoader& resourceLoader,
      const Config& conf = defaultConfig
    );

    Shader* load(const std::string& name);

    // Id32 getShaderId(const std::string& name);

    // Shader* getById(Id32 id);
    // Shader* get(const std::string& name);

private:
    Shader* findSlot();

    void addAttribute(Shader* shader, const ShaderAttributeConfig& cfg);
    void addUniform(Shader* shader, const ShaderUniformConfig& cfg);
    void addSampler(Shader* shader, const ShaderUniformConfig& cfg);

    void addUniformImpl(
      Shader* shader, const std::string& name, u32 size, ShaderUniform::Type type,
      ShaderScope scope, u32 setLocation, bool isSampler
    );

    RendererProxy& m_rendererProxy;
    const ResourceLoader& m_resourceLoader;
    Config m_conf;

    std::vector<Shader> m_shaders;
    std::unordered_map<std::string, Shader*> m_shaderByName;
};

}  // namespace sl
