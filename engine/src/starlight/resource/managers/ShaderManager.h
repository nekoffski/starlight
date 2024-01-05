#pragma once

#include <vector>

#include <kc/core/Singleton.hpp>

#include "starlight/renderer/gpu/Shader.h"
#include "starlight/renderer/gpu/ResourcePools.h"

#include "starlight/resource/resources/ShaderConfig.h"
#include "TextureManager.h"

namespace sl {

class ShaderManager {
public:
    explicit ShaderManager(
      ResourcePools& resourcePools, const TextureManager& textureManager
    );
    ~ShaderManager();

    Shader* load(const std::string& name);
    Shader* acquire(const std::string& name);

    void destroy(const std::string& name);
    void destroyAll();

private:
    ResourcePools& m_resourcePools;
    const TextureManager& m_textureManager;

    std::unordered_map<std::string, Shader*> m_shaders;
};

}  // namespace sl
