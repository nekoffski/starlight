#pragma once

#include <kcore/io/FileSystem.hh>

#include "Asset.hh"
#include "starlight/renderer/RendererProxy.hh"

namespace sl {

class ShaderAsset : public AssetRecord<ShaderHandle> {
   public:
    Result<void> reload();

    Result<ShaderInterface> interface();

   private:
    template <typename, typename>
    friend class AssetStore;

    explicit ShaderAsset(RendererProxy proxy);
    Result<void> initialize(const kc::Path& path);

    RendererProxy m_proxy;
    kc::Path m_path;
};

class ShaderRef : public AssetRef<ShaderAsset> {
   public:
    ShaderRef() = default;

    // Reads the original shader path again, then queues a replacement.
    Result<void> reload();

    // Queries the published shader through the renderer and waits for a
    // snapshot.
    Result<ShaderInterface> interface() const;

   private:
    template <typename, typename>
    friend class AssetStore;

    explicit ShaderRef(std::shared_ptr<ShaderAsset> record) noexcept
        : AssetRef(std::move(record)) {}
};

}  // namespace sl
