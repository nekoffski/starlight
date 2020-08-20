#pragma once

#include "starlight/ecs/Component.h"

#include <memory>

#include "starlight/platform/shader/Shader.h"
#include "starlight/rendering/data/ModelData.h"

namespace sl::scene::components {

struct ModelComponent : public ecs::Component {
    explicit ModelComponent(std::shared_ptr<geometry::Model> model)
        : modelData(model) {
        modelData.positions.push_back(math::Vec4(0.0f));
    }

    rendering::data::ModelData modelData;
};
}
