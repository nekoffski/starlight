#include "Shaders.h"

#include <memory>

#include "sl/asset/AssetLoader.hpp"

namespace sl::utils::globals {

Shaders::Shaders() {
    defaultCubemapShader = asset::AssetLoader::loadLocalPath<graphics::Shader>("/cubemap.vert", "/cubemap.frag");
    defaultModelShader = asset::AssetLoader::loadLocalPath<graphics::Shader>("/t.vert", "/t.frag");
}

}
