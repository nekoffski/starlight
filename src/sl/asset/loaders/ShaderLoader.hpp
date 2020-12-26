#pragma once

#include "sl/asset/loaders/AssetLoader.h"

#include "sl/core/Logger.h"
#include "sl/core/PathManager.hpp"
#include "sl/graphics/Shader.h"
#include "sl/graphics/ShaderCompiler.hpp"

namespace sl::asset::loaders {

using graphics::Shader;

template <>
struct AssetLoaderArgs<Shader> {
    std::string vertexPath;
    std::string fragmentPath;
    std::string geometryPath;
};

template <>
struct AssetLoader<Shader> {
    static std::shared_ptr<Shader> load(bool globalPath, AssetLoaderArgs<Shader> args) {
        std::string prefix = "";
        if (not globalPath)
            prefix += core::PathManager::get<Shader>();

        SL_DEBUG("loading shader: \n {} - {}/{}/{}", prefix, args.vertexPath, args.fragmentPath, args.geometryPath);

        auto shader = graphics::Shader::factory->create(
            prefix + args.vertexPath, prefix + args.fragmentPath, prefix + args.geometryPath);
        graphics::ShaderCompiler::compile(shader);
        return shader;
    }
};
}
