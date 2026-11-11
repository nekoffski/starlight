#pragma once

#include "Asset.hh"
#include "starlight/renderer/RendererProxy.hh"

namespace sl {

class SamplerAsset : public AssetRecord<SamplerHandle> {
   private:
    template <typename, typename>
    friend class AssetStore;

    explicit SamplerAsset(RendererProxy proxy);
    Result<void> initialize(const SamplerDescription& description);

    RendererProxy m_proxy;
};

class SamplerRef : public AssetRef<SamplerAsset> {
   public:
    SamplerRef() = default;

   private:
    template <typename, typename>
    friend class AssetStore;

    explicit SamplerRef(std::shared_ptr<SamplerAsset> record) noexcept
        : AssetRef(std::move(record)) {}
};

}  // namespace sl
