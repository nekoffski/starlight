#include "ShaderSource.h"

#include <fmt/core.h>
#include <kc/core/Log.h>

#include "starlight/core/Core.h"

namespace sl {

// TODO: this have to stay for know, remove as soon as refactoring shaders
std::optional<ShaderSource> ShaderSource::create(
  std::string_view name, const FileSystem& fs
) {
    const auto fullPath = fmt::format("{}/shaders/{}", SL_ASSETS_PATH, name);

    if (not fs.isFile(fullPath)) {
        LOG_WARN("Could not find shader file '{}'", fullPath);
        return {};
    }

    const auto source = fs.readFile(fullPath);
    return ShaderSource(fullPath, removeExtension(name), source);
}

ShaderSource::ShaderSource(
  std::string_view fullPath, std::string_view name, std::string_view source
) :
    m_source(source),
    data(m_source.data()), size(m_source.size()) {}

}  // namespace sl
