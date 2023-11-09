#pragma once

#include <vector>

#include <kc/core/Singleton.hpp>

#include "starlight/renderer/Shader.h"
#include "starlight/renderer/gpu/RendererProxy.h"

#include "starlight/resource/resources/ShaderConfig.h"
#include "TextureManager.h"

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
      RendererProxy& rendererProxy, const TextureManager& textureManager,
      const Config& conf = defaultConfig
    );

    Shader* load(const std::string& name);
    Shader* get(const std::string& name);

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
    const TextureManager& m_textureManager;
    Config m_conf;

    std::vector<Shader> m_shaders;
    std::unordered_map<std::string, Shader*> m_shaderByName;
};

}  // namespace sl
