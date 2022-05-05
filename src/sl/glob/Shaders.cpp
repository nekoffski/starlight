#include "Shaders.h"

#include <memory>

#include "Globals.h"
#include "sl/gpu/ShaderManager.h"

namespace sl::glob {

#define BIND_SHADER(shader) \
    { shader->name, shader }

Shaders::Shaders() {
    const std::string shadersPath = glob::Globals::get().config.paths.shaders;

    defaultCubemapShader = gpu::ShaderManager::get().load(
        shadersPath + "/Cubemap.vert", shadersPath + "/Cubemap.frag"
    );

    singleColorShader = gpu::ShaderManager::get().load(
        shadersPath + "/SingleColor.vert", shadersPath + "/SingleColor.frag"
    );

    defaultModelShader = gpu::ShaderManager::get().load(
        shadersPath + "/DefaultModelShader.vert", shadersPath + "/DefaultModelShader.frag"
    );
    defaultModelShader->name = "default-model-shader";

    pfxShader = gpu::ShaderManager::get().load(
        shadersPath + "/particle.vert", shadersPath + "/particle.frag"
    );
    pfxShader->name = "pfx-shader";

    shadersByName = {
        BIND_SHADER(defaultModelShader),
        BIND_SHADER(pfxShader),
    };
}

}  // namespace sl::glob
