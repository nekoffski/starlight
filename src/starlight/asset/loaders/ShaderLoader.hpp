#pragma once

#include <starlight/asset/loaders/AssetLoader.h>
#include <starlight/asset/PathManager.hpp>

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
        return Shader::create(PathManager::createGlobalPath<Shader>(args.vertexPath),
            PathManager::createGlobalPath<Shader>(args.fragmentPath), PathManager::createGlobalPath<Shader>(args.geometryPath));
    }
};
}
