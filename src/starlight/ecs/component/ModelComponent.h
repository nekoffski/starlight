#pragma once

#include <memory>

#include <starlight/ecs/component/Component.h>
#include <starlight/platform/shader/Shader.h>
#include <starlight/rendering/data/ModelRenderData.h>

namespace starl::ecs::component {

class ModelComponent : public Component {
public:
    ModelComponent(std::shared_ptr<geometry::Model> model, std::shared_ptr<platform::shader::Shader> shader)
        : m_renderData(std::make_shared<rendering::data::ModelRenderData>(model))
        , m_shader(std::move(shader)) {
    }

    ComponentType getType() const override {
        return ComponentType::MODEL;
    }

    std::shared_ptr<rendering::data::ModelRenderData> getRenderData() {
        return m_renderData;
    }

    std::shared_ptr<platform::shader::Shader> getShader() {
        return m_shader;
    }

private:
    // TODO this should be created on stack
    std::shared_ptr<rendering::data::ModelRenderData> m_renderData;
    std::shared_ptr<platform::shader::Shader> m_shader;
};
}