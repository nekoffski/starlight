#include "ModelLoader.h"

#include "starlight/core/log/Logger.h"
#include "starlight/platform/model/assimp/AssimpModelLoader.h"

namespace sl::platform::model {

std::unique_ptr<ModelLoader> ModelLoader::create() {
    SL_INFO("instancing assimp model loader");
    return std::make_unique<assimp::AssimpModelLoader>();
}
}