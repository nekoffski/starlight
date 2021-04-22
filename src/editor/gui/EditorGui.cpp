#include "EditorGui.h"

#include "sl/event/Emitter.hpp"
#include "sl/event/Event.h"

using namespace sl::core;

namespace editor::gui {

EditorGui::EditorGui(std::shared_ptr<SharedState> sharedState)
    : m_leftPanel(sharedState)
    , m_bottomPanel(sharedState)
    , m_rightPanel(sharedState)
    , sharedState(sharedState) {
}

void EditorGui::renderEditorGui(sl::gui::GuiApi& gui) {
    using namespace event;

    gui.pushTextColor(sl::gui::guiDefaultTextColor);

    if (gui.beginMainMenuBar()) {
        if (gui.beginMenu(ICON_FA_BARS " File")) {
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
    m_rightPanel.render(gui);

    gui.popColor();
}
}
