#include "AssetSystem.hh"

#include "loaders/ShaderLoader.hh"
#include "starlight/core/Core.hh"

namespace sl {

AssetSystem::AssetSystem(const Config& cfg, RendererProxy proxy)
    : m_cfg(cfg), m_proxy(proxy) {}

Result<ShaderRef> AssetSystem::loadShader(const kc::Path& path) {
    const auto fullPath = kc::Path::join(m_cfg.paths.assets, path);
    const auto description = ShaderLoader::loadShaderDescription(fullPath);

    if (not description) {
        log::warn("Could not load shader description: {}", description.error());
        return Error::unexpected(description.error());
    }

    ShaderRef shader{m_proxy, description.value(), m_eventLoop};

    if (shader.state() == BackendResourceState::failed) {
        return Error::unexpected(shader.error());
    }

    return shader;
}

}  // namespace sl
