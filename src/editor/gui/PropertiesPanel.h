#pragma once

#include <memory>

#include "sl/asset/AssetManager.h"
#include "sl/ecs/Entity.h"
#include "sl/gfx/Cubemap.h"
#include "sl/gui/GuiApi.h"

#include "Widget.h"

namespace editor::gui {

class PropertiesPanel : public Widget {
public:
    explicit PropertiesPanel(std::shared_ptr<SharedState> sharedState);
    void render(sl::gui::GuiApi& gui) override;

private:
    void showSceneProperties(sl::gui::GuiApi& gui);
    void showEntityProperties(sl::gui::GuiApi& gui);

    void addModel(bool load, sl::gui::GuiApi& gui);
    void addRenderer(bool load, sl::gui::GuiApi& gui);
    void addTransform(bool load, sl::gui::GuiApi& gui);
    void addPointLight(bool load, sl::gui::GuiApi& gui);
    void addDirectionalLight(bool load, sl::gui::GuiApi& gui);
    void addParticleEffect(bool load, sl::gui::GuiApi& gui);

    std::weak_ptr<sl::gfx::Cubemap> m_selectedCubemap;
};
}
