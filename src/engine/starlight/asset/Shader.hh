#pragma once

#include <kcore/io/FileSystem.hh>

#include "Asset.hh"
#include "starlight/renderer/RendererProxy.hh"

namespace sl {

class ShaderAsset : public AssetRecord<ShaderHandle> {
   public:
    ShaderAsset(RendererProxy proxy, kc::Path path);

    Result<void> reload();

   private:
    RendererProxy m_proxy;
    kc::Path m_path;
};

class ShaderRef : public AssetRef<ShaderAsset> {
   public:
    ShaderRef() = default;

    // Reads the original shader path again, then queues a replacement.
    Result<void> reload();

   private:
    template <typename, typename>
    friend class AssetStore;

    explicit ShaderRef(std::shared_ptr<ShaderAsset> record)
        : AssetRef(std::move(record)) {}
};

}  // namespace sl
