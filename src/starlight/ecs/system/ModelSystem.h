#pragma once

#include <starlight/ecs/component/ModelComponent.h>
#include <starlight/ecs/system/System.h>

// TODO: move FWD to another file
#include <starlight/rendering/renderer/ModelRenderer.h>

namespace starl::ecs::system {

class ModelSystem : public System {
public:
    void clean() {
        m_models.clear();
    }

    void processComponent(std::shared_ptr<component::Component>& component) override {
        processComponentImpl(std::dynamic_pointer_cast<component::ModelComponent>(component));
    }

    rendering::renderer::ShaderToModelRenderData& getModels() {
        return m_models;
    }

private:
    void processComponentImpl(std::shared_ptr<component::ModelComponent> component) {
        m_models[component->getShader()].push_back(component->getRenderData());
    }

    rendering::renderer::ShaderToModelRenderData m_models;
};
}
