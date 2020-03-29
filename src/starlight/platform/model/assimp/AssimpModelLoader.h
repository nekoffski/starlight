#pragma once

#include <starlight/platform/model/ModelLoader.h>

class aiNode;
class aiScene;

namespace starl::geometry {
class Model;
}

namespace starl::platform::model::assimp {
class AssimpMeshProcessor;

class AssimpModelLoader : public ModelLoader {

public:
    std::shared_ptr<geometry::Model> loadModel(std::string) override;

private:
    void processNode(aiNode*, const aiScene*, AssimpMeshProcessor&, std::shared_ptr<geometry::Model>&);
};
}
