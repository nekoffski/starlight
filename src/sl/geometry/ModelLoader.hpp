#pragma once

#include <memory>

#include "ModelLoaderImpl.h"

namespace sl::geometry {

struct ModelLoader {
    inline static std::unique_ptr<ModelLoaderImpl> impl = nullptr;

    static std::shared_ptr<Model> loadModel(const std::string& path) {
        return impl->loadModel(path);
    }
};
}
