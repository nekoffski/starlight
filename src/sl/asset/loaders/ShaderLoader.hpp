#pragma once

#include "sl/asset/loaders/AssetLoader.h"

#include "sl/core/log/Logger.h"
#include "sl/core/path/PathManager.hpp"
#include "sl/platform/shader/Shader.h"
#include "sl/platform/shader/ShaderCompiler.hpp"

namespace sl::asset::loaders {

using platform::shader::Shader;

template <>
struct AssetLoaderArgs<Shader> {
    std::string vertexPath;
    std::string fragmentPath;
    std::string geometryPath;
};

template <>
struct AssetLoader<Shader> {
    static std::shared_ptr<Shader> load(AssetLoaderArgs<Shader> args) {
        SL_DEBUG("loading shader: \n {}/{}/{}", args.vertexPath, args.fragmentPath, args.geometryPath);
        auto shader = Shader::create(core::path::PathManager::createGlobalPath<Shader>(args.vertexPath),
            core::path::PathManager::createGlobalPath<Shader>(args.fragmentPath), core::path::PathManager::createGlobalPath<Shader>(args.geometryPath));
        platform::shader::ShaderCompiler::compile(shader);
        return shader;
    }
};
}
