#include "Shader.hh"

#include <kcore/Pipeline.hh>

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
    const auto loadDescription = [this] {
        return ShaderLoader::loadShaderDescription(m_path);
    };
    const auto queueCreation = [this](const ShaderDescription& description) {
        return m_proxy.createShaderAsync(description);
    };
    const auto adoptFuture = [this](auto future) {
        adoptCreationFuture(std::move(future));
    };
    return kc::startPipeline(loadDescription)
        .then(queueCreation)
        .transform(adoptFuture)
        .recover([this](const Error& error) { return fail(error); })
        .result();
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
