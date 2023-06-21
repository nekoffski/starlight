#include "ShaderSource.h"

#include <fmt/core.h>
#include <kc/core/Log.h>

#include "Utils.h"

namespace sl {

std::optional<ShaderSource> ShaderSource::create(std::string_view name, const FileSystem& fs) {
    const auto fullPath = fmt::format("{}/shaders/{}", getAssetsBasePath(), name);

    if (not fs.isFile(fullPath)) {
        LOG_WARN("Could not find shader file '{}'", fullPath);
        return {};
    }

    const auto source = fs.readFile(fullPath);
    return ShaderSource(fullPath, removeExtension(name), source);
}

ShaderSource::ShaderSource(
    std::string_view fullPath, std::string_view name, std::string_view source
)
    : Resource(fullPath, name), m_source(source), data(m_source.data()), size(m_source.size()) {}

}  // namespace sl
