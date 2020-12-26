#pragma once

#include "sl/ecs/Component.h"

#include <memory>

#include "sl/graphics/Shader.h"
#include "sl/graphics/data/ModelData.h"

namespace sl::scene::components {

struct ModelComponent : public ecs::Component {
    explicit ModelComponent(std::shared_ptr<geometry::Model> model)
        : modelData(model) {
        modelData.positions.push_back(math::Vec4(0.0f));
    }

    graphics::data::ModelData modelData;
};
}
