#include "EditorGui.h"

#include "sl/event/Emitter.hpp"
#include "sl/event/Event.h"

using namespace sl::core;

namespace editor::gui {

EditorGui::EditorGui(std::shared_ptr<SharedState> sharedState)
    : m_leftPanel(sharedState)
    , m_bottomPanel(sharedState)
    , m_rightPanel(sharedState)
    , m_toolBar(sharedState)
    , sharedState(sharedState)
    , m_fileBrowser("editorGuiFileBrowser") {
}

void EditorGui::renderEditorGui(sl::gui::GuiApi& gui) {
    using namespace event;

    gui.pushTextColor(sl::gui::guiDefaultTextColor);

    std::optional<sl::gui::FileBrowser::Callback> callback;

    if (gui.beginMainMenuBar()) {
        if (gui.beginMenu(ICON_FA_BARS " File")) {
            if (gui.menuItem("Export scene"))
                callback = [](const std::string& path) -> void {
                    Emitter::emit<SerializeSceneEvent>(path);
                };

            if (gui.menuItem("Import scene"))
                callback = [](const std::string& path) -> void {
                    Emitter::emit<DeserializeSceneEvent>(path);
                };

            if (gui.menuItem("Quit"))
                Emitter::emit<QuitEvent>();

            gui.endMenu();
        }

        gui.endMainMenuBar();
    }

    if (callback.has_value()) {
        m_fileBrowser.open(gui, std::move(callback.value()));
        callback.reset();
    }

    m_fileBrowser.show(gui);

    m_leftPanel.render(gui);
    m_bottomPanel.render(gui);
    m_rightPanel.render(gui);
    m_toolBar.render(gui);

    gui.popColor();
}
}
