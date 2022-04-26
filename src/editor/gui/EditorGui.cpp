#include "EditorGui.h"

#include "sl/event/Event.h"
#include "sl/event/EventManager.h"

using namespace sl::core;

namespace editor::gui {

EditorGui::EditorGui(std::shared_ptr<SharedState> sharedState)
    : m_leftPanel(sharedState)
    , m_bottomPanel(sharedState)
    , m_rightPanel(sharedState)
    , m_toolBar(sharedState)
    , sharedState(sharedState)
    , m_fileBrowser("editorGuiFileBrowser") {}

void EditorGui::renderEditorGui() {
    using namespace event;

    sl::gui::pushTextColor(sl::gui::guiDefaultTextColor);

    std::optional<sl::gui::FileBrowser::Callback> callback;

    with_MainMenuBar {
        with_Menu(ICON_FA_BARS " File") {
            if (ImGui::MenuItem("Export scene")) {
                callback = [](const std::string& path) -> void {
                    EventManager::get().emit<SerializeSceneEvent>(path).toAll();
                };
            }

            if (ImGui::MenuItem("Import scene")) {
                callback = [](const std::string& path) -> void {
                    EventManager::get().emit<DeserializeSceneEvent>(path).toAll();
                };
            }

            if (ImGui::MenuItem("Close scene")) EventManager::get().emit<CloseSceneEvent>().toAll();

            if (ImGui::MenuItem("Quit")) EventManager::get().emit<QuitEvent>().toAll();
        }
    }

    if (callback.has_value()) {
        m_fileBrowser.open(std::move(callback.value()));
        callback.reset();
    }

    m_fileBrowser.show();

    m_leftPanel.render();
    m_bottomPanel.render();
    m_rightPanel.render();
    m_toolBar.render();

    sl::gui::popTextColor();
}
}  // namespace editor::gui
