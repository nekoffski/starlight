#include "MaterialConfig.h"

#include "Utils.hpp"

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
        auto root = kc::json::loadJson(fs.readFile(fullPath));
        MaterialConfig config;

        config.name         = name;
        config.diffuseColor = getFieldOr(root, "diffuse-color", defaultDiffuseColor);
        config.diffuseMap   = getFieldOr(root, "diffuse-map", defaultDiffuseMap);
        config.specularMap  = getFieldOr(root, "specular-map", defaultSpecularMap);
        config.normalMap    = getFieldOr(root, "normal-map", defaultNormalMap);
        config.shaderName   = getFieldOr(root, "shader-name", defaultShader);
        config.shininess    = getFieldOr(root, "shininess", defaultShininess);

        return config;
    } catch (kc::json::JsonError& e) {
        LOG_ERROR("Could not parse material '{}' file: {}", name, e.asString());
    }
    return {};
}
}  // namespace sl
