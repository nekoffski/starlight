#pragma once

#include "Asset.hh"
#include "starlight/renderer/RendererProxy.hh"

namespace sl {

class SamplerAsset : public AssetRecord<SamplerHandle> {
   public:
    static Result<std::shared_ptr<SamplerAsset>> create(
        RendererProxy& proxy, const SamplerDescription& description
    );
};

class SamplerRef : public AssetRef<SamplerAsset> {
   public:
    SamplerRef() = default;

   private:
    template <typename, typename>
    friend class AssetStore;

    explicit SamplerRef(std::shared_ptr<SamplerAsset> record)
        : AssetRef(std::move(record)) {}
};

}  // namespace sl
