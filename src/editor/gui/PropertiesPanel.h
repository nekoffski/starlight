#pragma once

#include <memory>

#include "sl/ecs/Entity.h"
#include "sl/gui/GuiApi.h"
#include "sl/graphics/Cubemap.h"

#include "Widget.h"
#include "editor/res/ResourceManager.h"

namespace editor::gui {

class PropertiesPanel : public Widget {
public:
    explicit PropertiesPanel(const WidgetPosition& position, res::ResourceManager& resourceManager, std::shared_ptr<sl::ecs::Entity>&);
    void render(sl::gui::GuiApi& gui) override;

	void setPosition(const WidgetPosition& position) {
        m_position = position;
    }

private:
    void showSceneProperties(sl::gui::GuiApi& gui);
    void showEntityProperties(sl::gui::GuiApi& gui);

    void addModel(bool load, sl::gui::GuiApi& gui);
    void addRenderer(bool load, sl::gui::GuiApi& gui);
    void addTransform(bool load, sl::gui::GuiApi& gui);
    void addPointLight(bool load, sl::gui::GuiApi& gui);
    void addDirectionalLight(bool load, sl::gui::GuiApi& gui);
	void addParticleEffect(bool load, sl::gui::GuiApi& gui);

    WidgetPosition m_position;
    res::ResourceManager& m_resourceManager;
    std::weak_ptr<sl::graphics::Cubemap> m_selectedCubemap;
    std::shared_ptr<sl::ecs::Entity>& m_selectedEntity;
};
}
