#include "Shaders.h"

#include <memory>

#include "sl/gfx/Shader.h"
#include "sl/utils/Globals.h"

namespace sl::utils::globals {

Shaders::Shaders() {
    defaultCubemapShader =
        gfx::Shader::load(
            GLOBALS().config.paths.shaders + "/cubemap.vert", GLOBALS().config.paths.shaders + "/cubemap.frag");

    defaultModelShader =
        gfx::Shader::load(
            GLOBALS().config.paths.shaders + "/t.vert", GLOBALS().config.paths.shaders + "/t.frag");
}

}
