#pragma once

#include <memory>

#include "Model.h"
#include "ModelLoader.h"
#include "sl/core/Singleton.hpp"

namespace sl::geom {

class GeometryManager : public core::Singleton<GeometryManager> {
public:
    std::shared_ptr<Model> loadModel(const std::string& path) {
        return m_modelLoader->load(path);
    }

    void setModelLoader(ModelLoader* modelLoader) {
        m_modelLoader = modelLoader;
    }

private:
    ModelLoader* m_modelLoader;
};

}