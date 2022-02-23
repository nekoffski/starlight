#pragma once

#include <kc/core/Singleton.hpp>
#include <memory>

#include "Model.h"
#include "ModelLoader.h"

namespace sl::geom {

class GeometryManager : public kc::core::Singleton<GeometryManager> {
   public:
    std::shared_ptr<Model> loadModel(const std::string& path) { return m_modelLoader->load(path); }

    void setModelLoader(ModelLoader* modelLoader) { m_modelLoader = modelLoader; }

   private:
    ModelLoader* m_modelLoader;
};

}  // namespace sl::geom