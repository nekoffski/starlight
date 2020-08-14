#pragma once

#include <memory>

namespace sl::geometry {
class Model;
}

namespace sl::platform::model {

class ModelLoader {
public:
    static std::unique_ptr<ModelLoader> create();

    virtual std::shared_ptr<geometry::Model> loadModel(std::string) = 0;
};
}