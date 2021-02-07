#include "EditorGui.h"
#include "sl/event/Event.h"

using namespace sl::core;

namespace editor::gui {

EditorGui::EditorGui(const Settings& settings, EntitiesVector& entities, sl::asset::AssetManager& assetManager)
    : m_leftPanel(settings, entities, assetManager)
    , m_bottomPanel(settings, assetManager) {
}

void EditorGui::setSettings(const Settings& settings) {
    m_leftPanel.setSettings(settings);
    m_bottomPanel.setSettings(settings);
}

void EditorGui::renderEditorGui(sl::gui::GuiApi& gui) {
    if (gui.beginMainMenuBar()) {
        if (gui.beginMenu("File")) {
            if (gui.menuItem("Export scene")) {
            }

            if (gui.menuItem("Import scene")) {
            }

            if (gui.menuItem("Quit"))
                event::Emitter::emit<event::QuitEvent>();

            gui.endMenu();
        }

        gui.endMainMenuBar();
    }

    m_leftPanel.render(gui);
    m_bottomPanel.render(gui);
}
}
