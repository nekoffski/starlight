#include "AssetSystem.hh"

#include <array>
#include <kcore/String.hh>

namespace sl {
namespace {

Str shaderCacheKey(const kc::Path& path) {
    kc::Path key{path.isAbsolute() ? "/" : ""};
    for (const auto& component : kc::split(path.str(), "/")) {
        if (not component.empty() && component != ".") {
            key = kc::Path::join(key, component);
        }
    }
    return key.str();
}

}  // namespace

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
    const auto fullPath = kc::Path::join(m_assetPath, path);
    const auto key = shaderCacheKey(fullPath);
    if (const auto found = m_shaderPaths.find(key);
        found != m_shaderPaths.end()) {
        if (auto record = found->second.lock()) {
            return ShaderStore::reference(std::move(record));
        }
    }
    const auto entry = m_shaderPaths.try_emplace(key).first;
    auto shader = m_shaders.create(fullPath);
    if (not shader) {
        m_shaderPaths.erase(entry);
        return Error::unexpected(shader.error());
    }
    entry->second = ShaderStore::weakReference(*shader);
    return shader;
}

Result<TextureRef> AssetSystem::createTexture(const TextureUploadData& data) {
    checkThread();
    if (m_closed) {
        return Error::unexpected(
            kc::ErrorCode::invalidArgument, "Asset system is closed"
        );
    }
    return m_textures.create(data);
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
    return m_samplers.create(description);
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
