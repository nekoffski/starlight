#include "MaterialConfig.h"

#include <fmt/core.h>
#include <kc/json/Json.h>
#include <kc/json/Utils.hpp>

#include "Utils.h"

namespace sl {

auto fieldFrom(auto& root) { return kc::json::fieldFrom<kc::json::JsonError>(root); }

std::optional<MaterialConfig> MaterialConfig::create(
    const std::string& name, const FileSystem& fs
) {
    const auto fullPath = fmt::format("{}/materials/{}", getAssetsBasePath(), name);

    if (not fs.isFile(fullPath)) {
        LOG_ERROR("Could not find file: '{}'", fullPath);
        return {};
    }

    MaterialConfig config(fullPath, removeExtension(name));

    const auto content = fs.readFile(fullPath);

    try {
        auto root = kc::json::loadJson(content);

        config.diffuseColor = fieldFrom(root).withName("diffuse-color").ofType<Vec4f>().get();
        config.diffuseMap   = fieldFrom(root).withName("diffuse-map").ofType<std::string>().get();

        return config;
    } catch (kc::json::JsonError& e) {
        LOG_ERROR("Could not parse material '{}' file: {}", name, e.what());
        return {};
    }
}

}  // namespace sl
