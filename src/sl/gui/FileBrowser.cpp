#include "FileBrowser.h"

// #include <fmt/core.h>

#include "sl/core/Logger.h"
#include "sl/gui/fonts/FontAwesome.h"

const std::string rootPath = "/home/nek0/kapik/projects/starlight";

namespace sl::gui {

FileBrowser::FileBrowser(const std::string& id, std::unique_ptr<core::FileSystem> fileSystem)
    : m_id(id)
    , m_fileSystem(std::move(fileSystem)) {
}

void FileBrowser::open(std::string* path, GuiApi& gui, std::optional<Callback> callback) {
    m_path = path;
    m_root = rootPath;

    m_history.clear();
    m_history.push_back(m_root);

    m_callback = callback;

    SL_INFO("Opening file dialog");
    gui.openPopUp(m_id);
}

void FileBrowser::show(GuiApi& gui) {
    constexpr float windowWidth = 700.0f;
    gui.setNextWindowSize({ windowWidth, 0.0f });

    if (gui.beginPopUp(m_id)) {
        SL_INFO("Displaying file dialog");

        handleHistory(gui);
        gui.breakLine();

        constexpr float mainContentRatio = 0.6f;
        const float mainContentHeight = 450.0f * mainContentRatio;

        gui.beginChild("##fileBrowserChild", math::Vec2 { 0.0f, mainContentHeight });
        handleFileExplorer(gui);
        gui.endChild();

        gui.breakLine();

        handleBottomPanel(gui);

        gui.endPopUp();
    }
}

void FileBrowser::handleHistory(GuiApi& gui) {
    for (auto entry = m_history.begin(); entry != m_history.end(); ++entry) {
        gui.sameLine();
        if (auto name = extractNameFromPath(*entry); gui.button(name)) {
            m_root = *entry;
            m_history = std::vector(m_history.begin(), std::next(entry));

            return;
        }
    }
}

void FileBrowser::handleFileExplorer(GuiApi& gui) {
    for (auto& entry : m_fileSystem->listDirectory(m_root)) {
        auto entryName = extractNameFromPath(entry);
        bool isDirectory = m_fileSystem->isDirectory(entry);

        auto tt = fmt::format("  {} {}", isDirectory ? ICON_FA_FOLDER_OPEN : ICON_FA_FILE, entryName);
        gui.displayText(tt);

        if (gui.isPreviousWidgetClicked()) {
            m_currentSelection = entry;

            if (isDirectory) {
                m_root = entry;
                m_history.push_back(entry);
            }
        }
    }
}

void FileBrowser::handleBottomPanel(GuiApi& gui) {
    if (gui.button(ICON_FA_CHECK_CIRCLE "  Ok")) {
        if (m_path != nullptr)
            *m_path = m_currentSelection;

        if (m_callback.has_value())
            std::invoke(m_callback.value(), m_currentSelection);

        gui.closeCurrentPopUp();
    }

    gui.sameLine();

    if (gui.button(ICON_FA_TIMES_CIRCLE "  Cancel"))
        gui.closeCurrentPopUp();

    gui.sameLine();
    gui.displayText("Full path: ");
    gui.sameLine();
    gui.inputText("##fileSelection", m_currentSelection);
}

std::string FileBrowser::extractNameFromPath(const std::string& path) {
    const auto fileNamePosition = path.find_last_of("/\\");
    return path.substr(fileNamePosition + 1);
}

}
