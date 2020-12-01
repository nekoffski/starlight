#pragma once

#include "BottomPanel.h"
#include "LeftPanel.h"
#include "Settings.h"
#include "sl/asset/AssetManager.hpp"
#include "sl/core/event/Event.h"
#include "sl/core/event/EventBus.h"
#include "sl/ecs/Entity.h"
#include "sl/gui/GUIProxy.h"
#include "sl/gui/Utils.hpp"
#include "sl/math/Vector.hpp"

using namespace sl::core;

namespace editor::gui {

class EditorGui {

public:
    explicit EditorGui(const Settings& settings, EntitiesVector& entities, res::ResourceManager& resourceManager)
        : m_leftPanel(settings, entities, resourceManager)
        , m_bottomPanel(settings, resourceManager) {
    }

    void renderEditorGui(sl::gui::GUIProxy& gui) {
        m_leftPanel.render(gui);
        m_bottomPanel.render(gui);
    }

private:
    LeftPanel m_leftPanel;
    BottomPanel m_bottomPanel;
};
}
