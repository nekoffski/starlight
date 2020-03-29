#pragma once

#include <memory>

namespace starl::geometry {
class Model;
}

namespace starl::platform::model {

class ModelLoader {
public:
    static std::unique_ptr<ModelLoader> create();

    virtual std::shared_ptr<geometry::Model> loadModel(std::string) = 0;
};
}