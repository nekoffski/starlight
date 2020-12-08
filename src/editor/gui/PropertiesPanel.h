#pragma once

#include <memory>

#include "sl/ecs/Entity.h"
#include "sl/platform/texture/Cubemap.h"

#include "Widget.h"
#include "editor/res/ResourceManager.h"

namespace editor::gui {

class PropertiesPanel : public Widget {
public:
    explicit PropertiesPanel(const WidgetPosition& position, res::ResourceManager& resourceManager, std::shared_ptr<sl::ecs::Entity>&);
    void render(sl::gui::GuiProxy& gui) override;

private:
    WidgetPosition m_position;
    res::ResourceManager& m_resourceManager;
    std::weak_ptr<sl::platform::texture::Cubemap> m_selectedCubemap;
    std::shared_ptr<sl::ecs::Entity>& m_selectedEntity;
};
}
