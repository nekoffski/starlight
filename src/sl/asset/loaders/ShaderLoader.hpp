#pragma once

#include "sl/asset/loaders/Loader.h"

#include "sl/core/Logger.h"
#include "sl/gfx/Shader.h"
#include "sl/gfx/ShaderCompiler.hpp"
#include "sl/utils/Globals.h"

namespace sl::asset::loaders {

using gfx::Shader;

template <>
struct LoaderArgs<Shader> {
    std::string vertexPath;
    std::string fragmentPath;
    std::string geomPath;
};

template <>
struct Loader<Shader> {
    static std::shared_ptr<Shader> load(bool globalPath, LoaderArgs<Shader> args) {
        std::string prefix = "";
        if (not globalPath)
            prefix += GLOBALS().config.paths.shaders;

        SL_DEBUG("loading shader: \n {} - {}/{}/{}", prefix, args.vertexPath, args.fragmentPath, args.geomPath);

        auto shader = gfx::Shader::factory->create(
            prefix + args.vertexPath, prefix + args.fragmentPath, prefix + args.geomPath);
        gfx::ShaderCompiler::compile(shader);
        return shader;
    }
};
}
