#pragma once

#include <kcore/Concepts.hh>
#include <unordered_map>

#include "AssetStore.hh"
#include "Sampler.hh"
#include "Shader.hh"
#include "Texture.hh"
#include "starlight/core/Config.hh"

namespace sl {

class AssetSystem : public kc::NonCopyable, public kc::NonMovable {
   public:
    explicit AssetSystem(const Config& cfg, RendererProxy proxy);
    ~AssetSystem();

    Result<ShaderRef> loadShader(const kc::Path& path);
    Result<TextureRef> createTexture(const TextureUploadData& data);
    Result<SamplerRef> createSampler(const SamplerDescription& description);

    Result<void> update();

    // Publishes queued creations/replacements and waits for requested releases.
    // Creation errors remain available through each asset reference.
    Result<void> waitForPending();

    // Release all references first. Waits for queued creation and destruction;
    // the renderer must remain running until this completes.
    Result<void> shutdown();

   private:
    void checkThread() const;
    bool hasReferences() const;

    using ShaderStore = AssetStore<ShaderAsset, ShaderRef>;
    using TextureStore = AssetStore<TextureAsset, TextureRef>;
    using SamplerStore = AssetStore<SamplerAsset, SamplerRef>;
    using ShaderPaths = std::unordered_map<Str, std::weak_ptr<ShaderAsset>>;

    kc::Path m_assetPath;
    RendererProxy m_proxy;
    ShaderStore m_shaders;
    TextureStore m_textures;
    SamplerStore m_samplers;
    ShaderPaths m_shaderPaths;
    const std::thread::id m_thread{std::this_thread::get_id()};
    bool m_closed{false};
};

}  // namespace sl
