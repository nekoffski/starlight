#include "Geometry.hh"

namespace sl {

GeometryAsset::GeometryAsset(RendererProxy proxy) : m_proxy(std::move(proxy)) {}

Result<void> GeometryAsset::initialize(const PrimitiveUploadData& data) {
    auto future = m_proxy.createPrimitiveAsync(data);
    if (not future) {
        return fail(future.error());
    }
    adoptCreationFuture(std::move(*future));
    return {};
}

}  // namespace sl
