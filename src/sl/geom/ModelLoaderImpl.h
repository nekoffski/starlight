#pragma once

#include <memory>

#include "Model.h"

namespace sl::geom {

class ModelLoaderImpl {
public:
    struct Factory {
        virtual std::unique_ptr<ModelLoaderImpl> create() = 0;
    };

    inline static std::unique_ptr<Factory> factory = nullptr;

    virtual std::shared_ptr<geom::Model> loadModel(const std::string&) = 0;
};
}
