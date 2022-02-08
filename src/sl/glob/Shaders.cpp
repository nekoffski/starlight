#include "Shaders.h"

#include <memory>

#include "Globals.h"
#include "sl/gfx/ShaderManager.h"

namespace sl::glob {

#define BIND_SHADER(shader) \
    { shader->name, shader }

Shaders::Shaders() {
    const std::string shadersPath = glob::Globals::get().config.paths.shaders;

    defaultCubemapShader = gfx::ShaderManager::get().load(
        shadersPath + "/Cubemap.vert", shadersPath + "/Cubemap.frag");

    singleColorShader = gfx::ShaderManager::get().load(
        shadersPath + "/SingleColor.vert", shadersPath + "/SingleColor.frag");

    defaultModelShader = gfx::ShaderManager::get().load(
        shadersPath + "/DefaultModelShader.vert", shadersPath + "/DefaultModelShader.frag");
    defaultModelShader->name = "default-model-shader";

    pfxShader = gfx::ShaderManager::get().load(
        shadersPath + "/particle.vert", shadersPath + "/particle.frag");
    pfxShader->name = "pfx-shader";

    shadersByName = {
        BIND_SHADER(defaultModelShader),
        BIND_SHADER(pfxShader),
    };
}

}
