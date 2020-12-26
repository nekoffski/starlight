#pragma once

#include <memory>

#include "Model.h"

namespace sl::geometry {

class ModelLoaderImpl {
public:
    struct Factory {
        virtual std::unique_ptr<ModelLoaderImpl> create() = 0;
    };

    inline static std::unique_ptr<Factory> factory = nullptr;

    virtual std::shared_ptr<geometry::Model> loadModel(std::string) = 0;
};
}
