#pragma once

#include <unordered_map>

#include <kc/core/Singleton.hpp>

#include "starlight/core/utils/FileSystem.h"
#include "starlight/core/memory/Memory.hpp"
#include "starlight/renderer/Material.h"
#include "starlight/renderer/gpu/RendererProxy.h"

#include "starlight/resource/resources/MaterialConfig.h"

#include "ShaderManager.h"

namespace sl {

class MaterialManager {
public:
    explicit MaterialManager(
      ShaderManager& shaderManager, TextureManager& textureManager,
      RendererProxy& rendererProxy
    );

    ~MaterialManager();

    Material* load(const std::string& name);
    Material* load(const MaterialConfig& config);

    Material* acquire(const std::string& name);
    Material* getDefaultMaterial();

    void createDefaultMaterial();

    void destroy(const std::string& name);
    void destroyAll();

private:
    ShaderManager& m_shaderManager;
    TextureManager& m_textureManager;
    RendererProxy& m_rendererProxy;

    std::unordered_map<std::string, Material> m_materials;
    Material m_defaultMaterial;
};

}  // namespace sl