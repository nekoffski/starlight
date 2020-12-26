#pragma once

#include "sl/geometry/ModelLoaderImpl.h"

class aiNode;
class aiScene;

namespace sl::geometry {
class Model;
}

namespace sl::platform::model {
class AssimpMeshProcessor;

class AssimpModelLoaderImpl : public geometry::ModelLoaderImpl {

public:
    std::shared_ptr<geometry::Model> loadModel(std::string) override;

private:
    void processNode(aiNode*, const aiScene*, AssimpMeshProcessor&, std::shared_ptr<geometry::Model>&);
};

struct AssimpModelLoaderImplFactory : geometry::ModelLoaderImpl::Factory {
    std::unique_ptr<geometry::ModelLoaderImpl> create() override {
        return std::make_unique<AssimpModelLoaderImpl>();
    }
};
}
