#include "ConfigLoader.h"

#include <kc/core/Log.h>

#include "sl/core/Errors.hpp"

namespace sl::cfg {

void ConfigLoader::processFields(const kc::json::Node& root) {
    using namespace kc::json;

    LOG_DEBUG("Processing config fields");
    LOG_TRACE("Processing paths config");
    auto paths = fieldFrom(root).withName("paths").asObject().get();

    // clang-format off
    config().paths.cubemaps = fieldFrom(paths).withName("cubemaps").ofType<std::string>().nonEmpty().get();
    config().paths.models   = fieldFrom(paths).withName("models")  .ofType<std::string>().nonEmpty().get();
    config().paths.shaders  = fieldFrom(paths).withName("shaders") .ofType<std::string>().nonEmpty().get();
    config().paths.textures = fieldFrom(paths).withName("textures").ofType<std::string>().nonEmpty().get();
    // clang-format on

    LOG_TRACE("Processing window config");
    auto window = fieldFrom(root).withName("window").asObject().get();

    // clang-format off
    config().window.height = fieldFrom(window).withName("height").ofType<unsigned int>().get();
    config().window.width  = fieldFrom(window).withName("width") .ofType<unsigned int>().get();
    config().window.name   = fieldFrom(window).withName("name") .ofType<std::string>()  .get();
    // clang-format on

    LOG_DEBUG("Config processed successfully");
}

}  // namespace sl::cfg