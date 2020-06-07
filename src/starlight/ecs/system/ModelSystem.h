#pragma once

#include <starlight/ecs/component/ModelComponent.h>
#include <starlight/ecs/system/System.h>
#include <starlight/math/Utils.hpp>

// TODO: move FWD to another file
#include <starlight/rendering/renderer/ModelRenderer.h>

namespace starl::ecs::system {

class ModelSystem : public System {
public:
    void clean() {
        m_models.clear();
    }

    void processComponent(std::shared_ptr<component::Component>& component, std::shared_ptr<entity::Entity>& entity) override {
        processComponentImpl(std::dynamic_pointer_cast<component::ModelComponent>(component), entity);
    }

    rendering::renderer::ShaderToModelRenderData& getModels() {
        return m_models;
    }

private:
    void processComponentImpl(std::shared_ptr<component::ModelComponent> component, const std::shared_ptr<entity::Entity>& entity) {
        auto translation = math::translate(entity->getPosition());
        auto rotation = math::createRotationMatrix(math::toRadians(entity->getOrientation()));
        *component->getRenderData()->modelMatrix = std::move(translation) * std::move(rotation);
        m_models[component->getShader()].push_back(component->getRenderData());
    }

    rendering::renderer::ShaderToModelRenderData m_models;
};
}
