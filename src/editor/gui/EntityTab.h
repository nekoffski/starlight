#pragma once

#include <memory>

#include "sl/ecs/Entity.h"
#include "sl/graphics/Cubemap.h"
#include "sl/gui/GuiApi.h"

#include "Widget.h"

namespace editor::gui {

class EntityTab : public Widget {
public:
    explicit EntityTab(std::shared_ptr<SharedState> sharedState);
    void render(sl::gui::GuiApi& gui) override;

private:
    void showEntityProperties(sl::gui::GuiApi& gui);

    void addModel(bool load, sl::gui::GuiApi& gui);
    void addRenderer(bool load, sl::gui::GuiApi& gui);
    void addTransform(bool load, sl::gui::GuiApi& gui);
    void addPointLight(bool load, sl::gui::GuiApi& gui);
    void addDirectionalLight(bool load, sl::gui::GuiApi& gui);
    void addParticleEffect(bool load, sl::gui::GuiApi& gui);

    std::weak_ptr<sl::graphics::Cubemap> m_selectedCubemap;
};
}
