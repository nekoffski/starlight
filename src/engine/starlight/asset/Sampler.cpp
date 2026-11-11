#include "Sampler.hh"

namespace sl {

Result<std::shared_ptr<SamplerAsset>> SamplerAsset::create(
    RendererProxy& proxy, const SamplerDescription& description
) {
    auto record = std::make_shared<SamplerAsset>();
    auto future = proxy.createSamplerAsync(description);
    if (not future) {
        return Error::unexpected(future.error());
    }
    record->adoptCreationFuture(std::move(*future));
    return record;
}

}  // namespace sl
