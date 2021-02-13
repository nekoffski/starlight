#include "EditorGui.h"

#include "sl/event/Emitter.hpp"
#include "sl/event/Event.h"

using namespace sl::core;

namespace editor::gui {

EditorGui::EditorGui(const Settings& settings, EntitiesVector& entities, sl::asset::AssetManager& assetManager)
    : m_leftPanel(settings, entities, assetManager)
    , m_bottomPanel(settings, assetManager)
    , m_assetManager(assetManager) {
}

void EditorGui::setSettings(const Settings& settings) {
    m_leftPanel.setSettings(settings);
    m_bottomPanel.setSettings(settings);
}

void EditorGui::renderEditorGui(sl::gui::GuiApi& gui) {
    using namespace event;

    if (gui.beginMainMenuBar()) {
        if (gui.beginMenu("File")) {
            if (gui.menuItem("Export scene")) {
                Emitter::emit<SerializeSceneEvent>();
            }

            if (gui.menuItem("Import scene")) {
                Emitter::emit<DeserializeSceneEvent>();
            }

            if (gui.menuItem("Quit"))
                Emitter::emit<QuitEvent>();

            gui.endMenu();
        }

        gui.endMainMenuBar();
    }

    m_leftPanel.render(gui);
    m_bottomPanel.render(gui);
}
}
