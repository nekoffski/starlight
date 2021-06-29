#include "Shaders.h"

#include <memory>

#include "sl/gfx/Shader.h"
#include "sl/utils/Globals.h"

namespace sl::utils::globals {

#define BIND_SHADER(shader) \
    { shader->name, shader }

Shaders::Shaders() {
    defaultCubemapShader = gfx::Shader::load(
        GLOBALS().config.paths.shaders + "/Cubemap.vert", GLOBALS().config.paths.shaders + "/Cubemap.frag");

    singleColorShader = gfx::Shader::load(
        GLOBALS().config.paths.shaders + "/SingleColor.vert", GLOBALS().config.paths.shaders + "/SingleColor.frag");

    defaultModelShader = gfx::Shader::load(
        GLOBALS().config.paths.shaders + "/DefaultModelShader.vert", GLOBALS().config.paths.shaders + "/DefaultModelShader.frag");
    defaultModelShader->name = "default-model-shader";

    pfxShader = gfx::Shader::load(
        GLOBALS().config.paths.shaders + "/particle.vert", GLOBALS().config.paths.shaders + "/particle.frag");
    pfxShader->name = "pfx-shader";

    shadersByName = {
        BIND_SHADER(defaultModelShader),
        BIND_SHADER(pfxShader)
    };
}

}
