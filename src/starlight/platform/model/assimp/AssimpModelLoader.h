#pragma once

#include "starlight/platform/model/ModelLoader.h"

class aiNode;
class aiScene;

namespace sl::geometry {
class Model;
}

namespace sl::platform::model::assimp {
class AssimpMeshProcessor;

class AssimpModelLoader : public ModelLoader {

public:
    std::shared_ptr<geometry::Model> loadModel(std::string) override;

private:
    void processNode(aiNode*, const aiScene*, AssimpMeshProcessor&, std::shared_ptr<geometry::Model>&);
};
}
