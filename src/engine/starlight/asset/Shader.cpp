#include "Shader.hh"

#include "loaders/ShaderLoader.hh"

namespace sl {

ShaderAsset::ShaderAsset(RendererProxy proxy) : m_proxy(std::move(proxy)) {}

Result<void> ShaderAsset::initialize(const kc::Path& path) {
    m_path = path;
    return reload();
}

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

Result<ShaderInterface> ShaderAsset::interface() {
    const auto current = handle();
    if (not current) {
        return Error::unexpected(current.error());
    }
    return m_proxy.getShaderInterface(*current);
}

Result<void> ShaderRef::reload() {
    if (not m_record) {
        return Error::unexpected(
            kc::ErrorCode::invalidArgument, "Empty shader reference"
        );
    }
    return m_record->reload();
}

Result<ShaderInterface> ShaderRef::interface() const {
    if (not m_record) {
        return Error::unexpected(
            kc::ErrorCode::invalidArgument, "Empty shader reference"
        );
    }
    return m_record->interface();
}

}  // namespace sl
