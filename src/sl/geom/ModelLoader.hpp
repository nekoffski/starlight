#pragma once

#include <memory>

#include "ModelLoaderImpl.h"

namespace sl::geom {

struct ModelLoader {
    inline static std::unique_ptr<ModelLoaderImpl> impl = nullptr;

    static std::shared_ptr<Model> load(const std::string& path) {
        return impl->loadModel(path);
    }
};
}
