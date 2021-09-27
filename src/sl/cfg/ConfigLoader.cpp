#include "ConfigLoader.h"

#include <kc/core/Log.h>

#include "sl/core/Errors.hpp"

namespace sl::cfg {

void ConfigLoader::processFields(const kc::json::Node& root) {
    using namespace kc::json;

    LOG_DEBUG("Processing config fields");
    LOG_TRACE("Processing paths config");
    auto paths = fieldFrom(root).ofName("paths").asObject().get();

    // clang-format off
    config().paths.cubemaps = fieldFrom(paths).ofName("cubemaps").ofType<std::string>().nonEmpty().get();
    config().paths.models   = fieldFrom(paths).ofName("models")  .ofType<std::string>().nonEmpty().get();
    config().paths.shaders  = fieldFrom(paths).ofName("shaders") .ofType<std::string>().nonEmpty().get();
    config().paths.textures = fieldFrom(paths).ofName("textures").ofType<std::string>().nonEmpty().get();
    // clang-format on

    LOG_TRACE("Processing window config");
    auto window = fieldFrom(root).ofName("window").asObject().get();

    // clang-format off
    config().window.height = fieldFrom(window).ofName("height").ofType<unsigned int>().get();
    config().window.width  = fieldFrom(window).ofName("width") .ofType<unsigned int>().get();
    config().window.name   = fieldFrom(window).ofName("name") .ofType<std::string>()  .get();
    // clang-format on

    LOG_DEBUG("Config processed successfully");
}

}