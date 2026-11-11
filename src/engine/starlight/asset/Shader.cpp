#include "Shader.hh"

#include "loaders/ShaderLoader.hh"

namespace sl {

ShaderAsset::ShaderAsset(RendererProxy proxy, kc::Path path)
    : m_proxy(std::move(proxy)), m_path(std::move(path)) {}

Result<void> ShaderAsset::reload() {
    checkThread();
    if (pending()) {
        return Error::unexpected(
            ErrorCode::assetBusy, "Shader creation or replacement is pending"
        );
    }
    auto description = ShaderLoader::loadShaderDescription(m_path);
    if (not description) {
        return fail(description.error());
    }
    auto future = m_proxy.createShaderAsync(*description);
    if (not future) {
        return fail(future.error());
    }
    adoptCreationFuture(std::move(*future));
    return {};
}

Result<void> ShaderRef::reload() {
    if (not m_record) {
        return Error::unexpected(
            kc::ErrorCode::invalidArgument, "Empty shader reference"
        );
    }
    return m_record->reload();
}

}  // namespace sl
