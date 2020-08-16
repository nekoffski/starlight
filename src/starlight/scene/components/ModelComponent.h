#pragma once

#include "starlight/ecs/Component.h"

#include <memory>

#include "starlight/platform/shader/Shader.h"
#include "starlight/rendering/data/ModelData.h"

namespace sl::scene::components {

class ModelComponent : public ecs::Component {
public:
    explicit ModelComponent(std::shared_ptr<geometry::Model> model)
        : m_modelData(std::make_shared<rendering::data::ModelData>(model)) {
        m_modelData->modelMatrices.push_back(math::Mat4(1.0f));
    }

    ModelComponent(const ModelComponent&) = default;

    std::shared_ptr<rendering::data::ModelData> getData() {
        return m_modelData;
    }

private:
    // TODO this should be created on stack
    std::shared_ptr<rendering::data::ModelData> m_modelData;
    std::shared_ptr<platform::shader::Shader> m_shader;
};
}
