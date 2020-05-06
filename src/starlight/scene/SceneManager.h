#pragma once

#include <starlight/ecs/system/ModelSystem.h>
#include <starlight/ecs/system/System.h>
#include <starlight/gui/Window.h>
#include <starlight/rendering/renderer/CubemapRenderer.h>
#include <starlight/rendering/renderer/ModelRenderer.h>
#include <starlight/scene/Scene.h>

#include <iostream>

namespace starl::scene {

struct SceneManagerArgs {
    std::shared_ptr<rendering::renderer::CubemapRenderer> cubemapRenderer;
    std::shared_ptr<rendering::renderer::ModelRenderer> modelRenderer;
};

class SceneManager {

public:
    explicit SceneManager(SceneManagerArgs&& args)
        : m_cubemapRenderer(std::move(args.cubemapRenderer))
        , m_modelRenderer(std::move(args.modelRenderer)) {
        m_modelSystem = std::make_shared<ecs::system::ModelSystem>();

        m_systems.insert({ ecs::component::ComponentType::MODEL, m_modelSystem });
    }

    void update(float deltaTime) {
        m_modelSystem->clean();

        for (auto& entity : m_scene->m_entities)
            processEntity(entity);
    }

    void setActiveScene(std::shared_ptr<Scene> scene) {
        m_scene = std::move(scene);
    }

    void render(const std::shared_ptr<framework::graphics::camera::Camera>& camera) {
        m_modelRenderer->render(m_modelSystem->getModels(), camera);

        if (const auto& skybox = m_scene->m_skybox; skybox)
            m_cubemapRenderer->render(skybox->cubemap, skybox->shader, camera);
    }

    void renderSceneGUI(gui::Window& window) {
        for (auto& entity : m_scene->m_entities) {
            window.displayText(entity->getName());
            if (window.isPreviousWidgetClicked())
                m_activeEntity = entity;
        }
    }

    void renderMainGUI(gui::Window& window) {
        if (auto entity = m_activeEntity.lock()) {
            if (window.beginTreeNode("Selected entity")) {
                entity->onGUI(window);
                window.popTreeNode();
            }
        }
    }

private:
    std::shared_ptr<Scene> m_scene;
    std::shared_ptr<rendering::renderer::CubemapRenderer> m_cubemapRenderer;
    std::shared_ptr<rendering::renderer::ModelRenderer> m_modelRenderer;
    std::shared_ptr<ecs::system::ModelSystem> m_modelSystem;
    std::unordered_map<ecs::component::ComponentType, std::shared_ptr<ecs::system::System>> m_systems;

    std::weak_ptr<ecs::entity::Entity> m_activeEntity;

    void processEntity(std::shared_ptr<ecs::entity::Entity>& entity) {
        for (auto& component : entity->getComponents())
            if (const auto type = component->getType(); m_systems.count(type) > 0)
                m_systems.at(type)->processComponent(component);

        for (auto& child : entity->getChilds())
            processEntity(child);
    }
};
}
