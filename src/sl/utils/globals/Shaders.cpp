#include "Shaders.h"

#include <memory>

#include "sl/asset/AssetLoader.hpp"

namespace sl::utils::globals {

Shaders::Shaders() {
    defaultCubemapShader = asset::AssetLoader::loadLocalPath<gfx::Shader>("/cubemap.vert", "/cubemap.frag");
    defaultModelShader = asset::AssetLoader::loadLocalPath<gfx::Shader>("/t.vert", "/t.frag");
}

}
