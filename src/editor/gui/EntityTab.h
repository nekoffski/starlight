#pragma once

#include <memory>
#include <unordered_map>

#include "sl/ecs/Entity.h"
#include "sl/gfx/Cubemap.h"
#include "sl/gui/EntityGui.h"
#include "sl/gui/ErrorDialog.h"


#include "Widget.h"

namespace editor::gui {

class EntityTab : public Widget {
public:
    explicit EntityTab(std::shared_ptr<SharedState> sharedState);
    void render() override;

private:
    void showEntityProperties();

    sl::gui::EntityGui m_entityGui;
    int m_selectedComponent;
    std::weak_ptr<sl::gfx::Cubemap> m_selectedCubemap;

    sl::gui::ErrorDialog m_errorDialog;

    std::unordered_map<std::string, std::string> m_entityNamePlacehoders;
};
}
