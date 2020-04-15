#include <starlight/platform/model/ModelLoader.h>

#include <starlight/platform/model/assimp/AssimpModelLoader.h>

#include <starlight/core/log/Logger.h>

namespace starl::platform::model {

std::unique_ptr<ModelLoader> ModelLoader::create() {
    LOG(INFO) << "instancing assimp model loader";
    return std::make_unique<assimp::AssimpModelLoader>();
}
}