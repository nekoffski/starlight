#pragma once

#include "sl/asset/loaders/Loader.h"

#include "sl/core/Logger.h"
#include "sl/graphics/Shader.h"
#include "sl/graphics/ShaderCompiler.hpp"
#include "sl/utils/Globals.h"

namespace sl::asset::loaders {

using graphics::Shader;

template <>
struct LoaderArgs<Shader> {
    std::string vertexPath;
    std::string fragmentPath;
    std::string geometryPath;
};

template <>
struct Loader<Shader> {
    static std::shared_ptr<Shader> load(bool globalPath, LoaderArgs<Shader> args) {
        std::string prefix = "";
        if (not globalPath)
            prefix += GLOBALS().config.paths.shaders;

        SL_DEBUG("loading shader: \n {} - {}/{}/{}", prefix, args.vertexPath, args.fragmentPath, args.geometryPath);

        auto shader = graphics::Shader::factory->create(
            prefix + args.vertexPath, prefix + args.fragmentPath, prefix + args.geometryPath);
        graphics::ShaderCompiler::compile(shader);
        return shader;
    }
};
}
