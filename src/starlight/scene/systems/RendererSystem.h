#pragma once

#include "starlight/core/perf/Profiler.hpp"
#include "starlight/rendering/RendererProxy.h"
#include "starlight/rendering/camera/Camera.h"
#include "starlight/scene/components/ModelComponent.h"
#include "starlight/scene/components/RendererComponent.h"
#include "starlight/scene/components/TransformComponent.h"

namespace sl::scene::systems {

class RendererSystem {
public:
    explicit RendererSystem(std::shared_ptr<rendering::RendererProxy> renderer)
        : m_renderer(renderer) {
    }

    void render(components::RendererComponent& component, std::shared_ptr<rendering::camera::Camera> camera) {
        component.shader->enable();
        component.shader->setUniform("view", camera->getViewMatrix());
        component.shader->setUniform("cameraPosition", camera->getPosition());

        auto& model = component.entity->getComponent<components::ModelComponent>();
        auto transformComponent = component.entity->getComponent<components::TransformComponent>();

        m_renderer->renderModels(component.shader, model.modelData, transformComponent());

        component.shader->disable();
    }

private:
    std::shared_ptr<rendering::RendererProxy> m_renderer;
};
}