#pragma once

#include <memory>

#include "sl/ecs/Entity.h"
#include "sl/gfx/Cubemap.h"
#include "sl/gui/GuiApi.h"

#include "Widget.h"

namespace editor::gui {

class EntityTab : public Widget {
public:
    explicit EntityTab(std::shared_ptr<SharedState> sharedState);
    void render(sl::gui::GuiApi& gui) override;

private:
    void showEntityProperties(sl::gui::GuiApi& gui);

    void addModel(bool load, sl::ecs::Entity& entity, sl::gui::GuiApi& gui);
    void addRenderer(bool load, sl::ecs::Entity& entity, sl::gui::GuiApi& gui);
    void addTransform(bool load, sl::ecs::Entity& entity, sl::gui::GuiApi& gui);
    void addPointLight(bool load, sl::ecs::Entity& entity, sl::gui::GuiApi& gui);
    void addDirectionalLight(bool load, sl::ecs::Entity& entity, sl::gui::GuiApi& gui);
    void addParticleEffect(bool load, sl::ecs::Entity& entity, sl::gui::GuiApi& gui);

    std::weak_ptr<sl::gfx::Cubemap> m_selectedCubemap;
};
}
