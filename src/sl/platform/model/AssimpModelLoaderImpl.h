#pragma once

#include "sl/geom/ModelLoaderImpl.h"

class aiNode;
class aiScene;

namespace sl::geom {
class Model;
}

namespace sl::platform::model {
class AssimpMeshProcessor;

class AssimpModelLoaderImpl : public geom::ModelLoaderImpl {

public:
    std::shared_ptr<geom::Model> loadModel(const std::string&) override;

private:
    void processNode(aiNode*, const aiScene*, AssimpMeshProcessor&, std::shared_ptr<geom::Model>&);
};

struct AssimpModelLoaderImplFactory : geom::ModelLoaderImpl::Factory {
    std::unique_ptr<geom::ModelLoaderImpl> create() override {
        return std::make_unique<AssimpModelLoaderImpl>();
    }
};
}
