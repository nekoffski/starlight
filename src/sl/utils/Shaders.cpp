#include "Shaders.h"

#include <memory>

#include "sl/gfx/ShaderManager.h"
#include "sl/utils/Globals.h"

namespace sl::utils {

#define BIND_SHADER(shader) \
    { shader->name, shader }

Shaders::Shaders() {
    defaultCubemapShader = gfx::ShaderManager::get()->load(
        GLOBALS().config.paths.shaders + "/Cubemap.vert", GLOBALS().config.paths.shaders + "/Cubemap.frag");

    singleColorShader = gfx::ShaderManager::get()->load(
        GLOBALS().config.paths.shaders + "/SingleColor.vert", GLOBALS().config.paths.shaders + "/SingleColor.frag");

    defaultModelShader = gfx::ShaderManager::get()->load(
        GLOBALS().config.paths.shaders + "/DefaultModelShader.vert", GLOBALS().config.paths.shaders + "/DefaultModelShader.frag");
    defaultModelShader->name = "default-model-shader";

    pfxShader = gfx::ShaderManager::get()->load(
        GLOBALS().config.paths.shaders + "/particle.vert", GLOBALS().config.paths.shaders + "/particle.frag");
    pfxShader->name = "pfx-shader";

    shadersByName = {
        BIND_SHADER(defaultModelShader),
        BIND_SHADER(pfxShader)
    };
}

}
