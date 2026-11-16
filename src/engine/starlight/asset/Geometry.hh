#pragma once

#include "Asset.hh"
#include "starlight/renderer/RendererProxy.hh"

namespace sl {

class GeometryAsset : public AssetRecord<PrimitiveHandle> {
   private:
    template <typename, typename>
    friend class AssetStore;

    explicit GeometryAsset(RendererProxy proxy);
    Result<void> initialize(const PrimitiveUploadData& data);

    RendererProxy m_proxy;
};

class GeometryRef : public AssetRef<GeometryAsset> {
   public:
    GeometryRef() = default;

   private:
    template <typename, typename>
    friend class AssetStore;

    explicit GeometryRef(std::shared_ptr<GeometryAsset> record) noexcept
        : AssetRef(std::move(record)) {}
};

}  // namespace sl
