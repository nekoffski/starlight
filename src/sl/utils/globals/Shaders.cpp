#include "Shaders.h"

#include <memory>

#include "sl/gfx/Shader.h"
#include "sl/utils/Globals.h"

namespace sl::utils::globals {

#define BIND_SHADER(shader) \
    { shader->name, shader }

Shaders::Shaders() {
    defaultCubemapShader = gfx::Shader::load(
        GLOBALS().config.paths.shaders + "/cubemap.vert", GLOBALS().config.paths.shaders + "/cubemap.frag");

    singleColorShader = gfx::Shader::load(
        GLOBALS().config.paths.shaders + "/single_color.vert", GLOBALS().config.paths.shaders + "/single_color.frag");

    defaultModelShader = gfx::Shader::load(
        GLOBALS().config.paths.shaders + "/t.vert", GLOBALS().config.paths.shaders + "/t.frag");
    defaultModelShader->name = "default-model-shader";

    shadersByName = {
        BIND_SHADER(defaultModelShader)
    };
}

}
