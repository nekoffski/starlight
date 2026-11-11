#include "AssetSystem.hh"

#include <array>

namespace sl {

AssetSystem::AssetSystem(const Config& cfg, RendererProxy proxy)
    : m_assetPath(cfg.paths.assets),
      m_proxy(std::move(proxy)),
      m_shaders(m_proxy, &RendererProxy::destroyShader),
      m_textures(m_proxy, &RendererProxy::destroyTexture),
      m_samplers(m_proxy, &RendererProxy::destroySampler) {}

AssetSystem::~AssetSystem() { log::expect(shutdown()); }

void AssetSystem::checkThread() const {
    log::expect(
        m_thread == std::this_thread::get_id(),
        "AssetSystem requires its engine thread"
    );
}

bool AssetSystem::hasReferences() const {
    return m_shaders.hasReferences() || m_textures.hasReferences() ||
           m_samplers.hasReferences();
}

Result<ShaderRef> AssetSystem::loadShader(const kc::Path& path) {
    checkThread();
    if (m_closed) {
        return Error::unexpected(
            kc::ErrorCode::invalidArgument, "Asset system is closed"
        );
    }
    const auto fullPath = kc::Path::join(m_assetPath, path).normalized();
    if (const auto found = m_shaderPaths.find(fullPath.str());
        found != m_shaderPaths.end()) {
        if (auto record = found->second.lock()) {
            return ShaderStore::reference(std::move(record));
        }
    }
    auto record = std::make_shared<ShaderAsset>(m_proxy, fullPath);
    const auto r = record->reload();
    if (not r) {
        return Error::unexpected(r.error());
    }
    m_shaderPaths[fullPath.str()] = record;
    return m_shaders.add(std::move(record));
}

Result<TextureRef> AssetSystem::createTexture(const TextureUploadData& data) {
    checkThread();
    if (m_closed) {
        return Error::unexpected(
            kc::ErrorCode::invalidArgument, "Asset system is closed"
        );
    }
    auto record = std::make_shared<TextureAsset>(m_proxy);
    const auto r = record->replace(data);
    if (not r) {
        return Error::unexpected(r.error());
    }
    return m_textures.add(std::move(record));
}

Result<SamplerRef> AssetSystem::createSampler(
    const SamplerDescription& description
) {
    checkThread();
    if (m_closed) {
        return Error::unexpected(
            kc::ErrorCode::invalidArgument, "Asset system is closed"
        );
    }
    auto record = SamplerAsset::create(m_proxy, description);
    if (not record) {
        return Error::unexpected(record.error());
    }
    return m_samplers.add(std::move(*record));
}

Result<void> AssetSystem::update() {
    checkThread();
    if (m_closed) {
        return {};
    }
    const std::array results{
        m_shaders.update(), m_textures.update(), m_samplers.update()
    };
    std::erase_if(m_shaderPaths, [](const auto& entry) {
        return entry.second.expired();
    });
    for (const auto& r : results) {
        if (not r) {
            return r;
        }
    }
    return {};
}

Result<void> AssetSystem::waitForPending() {
    checkThread();
    if (m_closed) {
        return {};
    }
    // The first barrier completes creation; the second completes cleanup.
    auto r = m_proxy.flushRenderer();
    if (not r) {
        return r;
    }
    r = update();
    if (not r) {
        return r;
    }
    return m_proxy.flushRenderer();
}

Result<void> AssetSystem::shutdown() {
    checkThread();
    if (m_closed) {
        return {};
    }
    if (hasReferences()) {
        return Error::unexpected(
            ErrorCode::assetInUse,
            "Release all asset references before shutting down AssetSystem"
        );
    }
    const auto r = waitForPending();
    if (not r) {
        return r;
    }
    m_closed = true;
    return {};
}

}  // namespace sl
