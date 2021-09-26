#include "Shaders.h"

#include <memory>

#include "Globals.h"
#include "sl/gfx/ShaderManager.h"

namespace sl::glob {

#define BIND_SHADER(shader) \
    { shader->name, shader }

Shaders::Shaders() {
    defaultCubemapShader = gfx::ShaderManager::get()->load(
        glob::Globals::get()->config.paths.shaders + "/Cubemap.vert", glob::Globals::get()->config.paths.shaders + "/Cubemap.frag");

    singleColorShader = gfx::ShaderManager::get()->load(
        glob::Globals::get()->config.paths.shaders + "/SingleColor.vert", glob::Globals::get()->config.paths.shaders + "/SingleColor.frag");

    defaultModelShader = gfx::ShaderManager::get()->load(
        glob::Globals::get()->config.paths.shaders + "/DefaultModelShader.vert", glob::Globals::get()->config.paths.shaders + "/DefaultModelShader.frag");
    defaultModelShader->name = "default-model-shader";

    pfxShader = gfx::ShaderManager::get()->load(
        glob::Globals::get()->config.paths.shaders + "/particle.vert", glob::Globals::get()->config.paths.shaders + "/particle.frag");
    pfxShader->name = "pfx-shader";

    shadersByName = {
        BIND_SHADER(defaultModelShader),
        BIND_SHADER(pfxShader)
    };
}

}
