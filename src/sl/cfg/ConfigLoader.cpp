#include "ConfigLoader.h"

#include "sl/core/Errors.hpp"

namespace sl::cfg {

Config ConfigLoader::loadFromFile(const std::string& path, const kc::core::FileSystem& fileSystem) && {
    if (not fileSystem.isFile(path))
        throw core::ConfigError { "Could not find config file: " + path };

    auto configJson = kc::json::loadJson(fileSystem.readFile(path));

    Config config;
    config.paths = processPaths(configJson);

    return config;
}

Config::Paths ConfigLoader::processPaths(kc::json::Node& root) {
    if (not root.isMember("paths"))
        raise("Config does not contain paths key.");

    auto& pathsJson = root["paths"];

    Config::Paths paths;

    auto processPath = [&root, &pathsJson, this](const std::string& path) -> std::string {
        if (not pathsJson.isMember(path))
            raise("Config does not contain " + path + " path.");

        auto& pathJson = pathsJson[path];

        if (not pathJson.isString())
            raise("Config record for paths::" + path + " has invalid type.");

        return pathJson.asString();
    };

    paths.shaders = processPath("shaders");
    paths.models = processPath("models");
    paths.textures = processPath("textures");
    paths.cubemaps = processPath("cubemaps");

    return paths;
}

void ConfigLoader::raise(const std::string& reason) {
    throw core::ConfigError { reason };
}

}