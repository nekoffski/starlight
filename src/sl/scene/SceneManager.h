#pragma once

#include "sl/ecs/Entity.h"
#include "sl/gui/Window.h"
#include "sl/rendering/RendererProxy.h"
#include "sl/rendering/renderer/CubemapRenderer.h"
#include "sl/rendering/renderer/ModelRenderer.h"
#include "sl/scene/Scene.h"
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
