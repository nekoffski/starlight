#include "ModelLoader.h"

#include "sl/core/Logger.h"
#include "sl/platform/model/assimp/AssimpModelLoader.h"

namespace sl::platform::model {

std::unique_ptr<ModelLoader> ModelLoader::create() {
    SL_INFO("instancing assimp model loader");
    return std::make_unique<assimp::AssimpModelLoader>();
}
}