#include "MaterialConfig.h"

#include "starlight/core/Json.hpp"

namespace sl {

MaterialConfig MaterialConfig::createDefault(const std::string& name) {
    return MaterialConfig{
        .name         = name,
        .diffuseColor = defaultDiffuseColor,
        .shininess    = defaultShininess,
        .diffuseMap   = defaultDiffuseMap,
        .specularMap  = defaultSpecularMap,
        .normalMap    = defaultNormalMap,
        .shaderName   = defaultShader
    };
}

std::optional<MaterialConfig> MaterialConfig::load(
  const std::string& name, const std::string& materialsPath, const FileSystem& fs
) {
    const auto fullPath = fmt::format("{}/{}.json", materialsPath, name);

    LOG_TRACE("Loading material config file: {}", fullPath);

    if (not fs.isFile(fullPath)) {
        LOG_ERROR("Could not find file: '{}'", fullPath);
        return {};
    }

    try {
        const auto root = kc::json::loadJson(fs.readFile(fullPath));

        return MaterialConfig{
            .name         = name,
            .diffuseColor = getFieldOr(root, "diffuse-color", defaultDiffuseColor),
            .shininess    = getFieldOr(root, "shininess", defaultShininess),
            .diffuseMap   = getFieldOr(root, "diffuse-map", defaultDiffuseMap),
            .specularMap  = getFieldOr(root, "specular-map", defaultSpecularMap),
            .normalMap    = getFieldOr(root, "normal-map", defaultNormalMap),
            .shaderName   = getFieldOr(root, "shader-name", defaultShader)
        };
    } catch (kc::json::JsonError& e) {
        LOG_ERROR("Could not parse material '{}' file: {}", name, e.asString());
    }
    return {};
}
}  // namespace sl
