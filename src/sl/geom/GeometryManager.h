#pragma once

#include <memory>

#include <kc/core/Singleton.hpp>

#include "Model.h"
#include "ModelLoader.h"

namespace sl::geom {

class GeometryManager : public kc::core::Singleton<GeometryManager> {
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