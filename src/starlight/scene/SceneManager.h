#pragma once

#include "starlight/ecs/system/MeshGridSystem.h"
#include "starlight/ecs/system/ModelSystem.h"
#include "starlight/ecs/system/PFXSystem.h"
#include "starlight/ecs/system/System.h"
#include "starlight/ecs/system/TransformSystem.h"
#include "starlight/gui/Window.h"
#include "starlight/rendering/RendererProxy.h"
#include "starlight/rendering/renderer/CubemapRenderer.h"
#include "starlight/rendering/renderer/ModelRenderer.h"
#include "starlight/scene/Scene.h"

namespace sl::scene {

class SceneManager {

public:
    explicit SceneManager(std::shared_ptr<rendering::RendererProxy>);

    void update(float);
    void setActiveScene(std::shared_ptr<Scene>);
    void render(const std::shared_ptr<rendering::camera::Camera>&);
    void renderSceneGUI(gui::Window&);
    void renderMainGUI(gui::Window&);

private:
    std::shared_ptr<Scene> m_scene;
    std::shared_ptr<rendering::RendererProxy> m_renderer;
    std::unordered_map<ecs::component::ComponentType, std::shared_ptr<ecs::system::System>> m_systems;

    std::weak_ptr<ecs::entity::Entity> m_activeEntity;
};
}
