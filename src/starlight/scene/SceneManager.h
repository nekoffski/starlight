#pragma once

#include "starlight/ecs/Entity.h"
#include "starlight/gui/Window.h"
#include "starlight/rendering/RendererProxy.h"
#include "starlight/rendering/renderer/CubemapRenderer.h"
#include "starlight/rendering/renderer/ModelRenderer.h"
#include "starlight/scene/Scene.h"
#include "systems/LightSystem.h"
#include "systems/PFXSystem.h"
#include "systems/RendererSystem.h"

namespace sl::scene {

class SceneManager {

public:
    explicit SceneManager(std::shared_ptr<rendering::RendererProxy>);

    void update(float);
    void setActiveScene(std::shared_ptr<Scene>);
    void render(const std::shared_ptr<rendering::camera::Camera>);
    void renderSceneGUI(gui::Window&);
    void renderMainGUI(gui::Window&);

private:
    std::shared_ptr<Scene> m_scene;
    std::shared_ptr<rendering::RendererProxy> m_renderer;

    systems::RendererSystem m_rendererSystem;
    systems::LightSystem m_lightSystem;
    systems::PFXSystem m_pfxSystem;

    std::weak_ptr<ecs::Entity> m_activeEntity;
};
}
