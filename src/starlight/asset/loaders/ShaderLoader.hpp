#pragma once

#include <starlight/asset/loaders/AssetLoader.h>
#include <starlight/core/path/PathManager.hpp>

#include <starlight/core/log/Logger.h>
#include <starlight/platform/shader/Shader.h>
#include <starlight/platform/shader/ShaderCompiler.hpp>

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
        LOG(DEBUG) << "loading shader: " << args.vertexPath << ", " << args.fragmentPath << ", " << args.geometryPath;
        auto shader = Shader::create(core::path::PathManager::createGlobalPath<Shader>(args.vertexPath),
            core::path::PathManager::createGlobalPath<Shader>(args.fragmentPath), core::path::PathManager::createGlobalPath<Shader>(args.geometryPath));
        platform::shader::ShaderCompiler::compile(shader);
        return shader;
    }
};
}
