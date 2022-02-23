#pragma once

#include "sl/geom/ModelLoader.h"

class aiNode;
class aiScene;

namespace sl::geom {
class Model;
}

namespace sl::platform::assimp {

class AssimpMeshProcessor;

class AssimpModelLoader : public geom::ModelLoader {
   public:
    std::shared_ptr<geom::Model> load(const std::string&) override;

   private:
    void processNode(aiNode*, const aiScene*, AssimpMeshProcessor&, std::shared_ptr<geom::Model>&);
};

}  // namespace sl::platform::assimp
