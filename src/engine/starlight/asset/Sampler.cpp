#include "Sampler.hh"

namespace sl {

SamplerAsset::SamplerAsset(RendererProxy proxy) : m_proxy(std::move(proxy)) {}

Result<void> SamplerAsset::initialize(const SamplerDescription& description) {
    auto future = m_proxy.createSamplerAsync(description);
    if (not future) {
        return fail(future.error());
    }
    adoptCreationFuture(std::move(*future));
    return {};
}

}  // namespace sl
