#pragma once

#include "SceneManager.h"

#include "sl/ecs/Entity.h"
#include "sl/gui/Panel.h"
#include "sl/rendering/Renderer.h"
#include "sl/scene/Scene3D.h"
#include "systems/LightSystem.h"
#include "systems/ParticleEffectSystem.h"
#include "systems/RendererSystem.h"
#include "systems/SkyboxSystem.h"
#include "systems/ShadowSystem.h"

namespace sl::scene {

class SceneManager3D : public SceneManager {

public:
    explicit SceneManager3D(std::shared_ptr<rendering::Renderer>);

    void update(float) override;
    void setActiveScene(std::shared_ptr<Scene>) override;
    void render() override;
    void renderSceneGUI(gui::Panel&) override;
    void renderMainGUI(gui::Panel&) override;

private:
    std::shared_ptr<Scene3D> m_scene;
    std::shared_ptr<rendering::Renderer> m_renderer;

    systems::RendererSystem m_rendererSystem;
    systems::LightSystem m_lightSystem;
    systems::ParticleEffectSystem m_pfxSystem;
	systems::SkyboxSystem m_skyboxSystem;
	systems::ShadowSystem m_shadowSystem;

    std::weak_ptr<ecs::Entity> m_activeEntity;
};
}
