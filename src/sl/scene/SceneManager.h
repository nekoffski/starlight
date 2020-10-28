#pragma once

#include "sl/ecs/Entity.h"
#include "sl/gui/Window.h"
#include "sl/rendering/Renderer.h"
#include "sl/scene/Scene.h"
#include "systems/LightSystem.h"
#include "systems/ParticleEffectSystem.h"
#include "systems/RendererSystem.h"
#include "systems/SkyboxSystem.h"

namespace sl::scene {

class SceneManager {

public:
    explicit SceneManager(std::shared_ptr<rendering::Renderer>);

    void update(float);
    void setActiveScene(std::shared_ptr<Scene>);
    void render(std::shared_ptr<rendering::camera::Camera>);
    void renderSceneGUI(gui::Window&);
    void renderMainGUI(gui::Window&);

private:
    std::shared_ptr<Scene> m_scene;
    std::shared_ptr<rendering::Renderer> m_renderer;

    systems::RendererSystem m_rendererSystem;
    systems::LightSystem m_lightSystem;
    systems::ParticleEffectSystem m_pfxSystem;
	systems::SkyboxSystem m_skyboxSystem;

    std::weak_ptr<ecs::Entity> m_activeEntity;
};
}
