#pragma once

#include "Asset.hh"
#include "starlight/renderer/RendererProxy.hh"

namespace sl {

class TextureAsset : public AssetRecord<TextureHandle> {
   public:
    explicit TextureAsset(RendererProxy proxy);

    Result<void> replace(const TextureUploadData& data);

   private:
    RendererProxy m_proxy;
};

class TextureRef : public AssetRef<TextureAsset> {
   public:
    TextureRef() = default;

    Result<void> replace(const TextureUploadData& data);

   private:
    template <typename, typename>
    friend class AssetStore;

    explicit TextureRef(std::shared_ptr<TextureAsset> record)
        : AssetRef(std::move(record)) {}
};

Result<TextureUploadData> makeCheckerboard(u32 size, u32 cellSize);

}  // namespace sl
