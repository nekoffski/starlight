#pragma once

#include <starlight/asset/PathManager.hpp>
#include <starlight/asset/loaders/AssetLoader.h>

#include <starlight/core/log/Logger.h>
#include <starlight/platform/shader/Shader.h>

namespace starl::asset::loaders {

using platform::shader::Shader;

template <>
struct AssetLoaderArgs<Shader> {
    std::string vertexPath;
    std::string fragmentPath;
    std::string geometryPath;
};

class ShaderLoader : public AssetLoader<Shader> {
public:
    std::shared_ptr<Shader> load(AssetLoaderArgs<Shader> args) {
        logger->trace("loading shader: {}, {}, {}", args.vertexPath, args.fragmentPath, args.geometryPath);
        return Shader::create(PathManager::createGlobalPath<Shader>(args.vertexPath),
            PathManager::createGlobalPath<Shader>(args.fragmentPath), PathManager::createGlobalPath<Shader>(args.geometryPath));
    }

private:
    Logger logger{ starl::core::log::createLogger("ShaderLoader") };
};
}
