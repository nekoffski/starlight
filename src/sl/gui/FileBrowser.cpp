#include "FileBrowser.h"

#include "sl/gui/GuiStyle.h"
#include "sl/gui/fonts/FontAwesome.h"
#include <kc/core/Log.h>

const std::string rootPath = "/home/nek0/kapik/projects/starlight";

namespace sl::gui {

FileBrowser::FileBrowser(const std::string& id, std::unique_ptr<kc::core::FileSystem> fileSystem)
    : m_id(id)
    , m_fileSystem(std::move(fileSystem)) {
}

void FileBrowser::open(GuiApi& gui, Callback&& callback) {
    m_currentSelection = rootPath;

    m_history.clear();
    m_history.push_back(m_currentSelection);

    m_callback = callback;

    LOG_DEBUG("Opening file browser");
    gui.openPopUp(m_id);
}

void FileBrowser::show(GuiApi& gui) {
    constexpr float windowWidth = 700.0f;
    gui.setNextWindowSize({ windowWidth, 0.0f });

    if (gui.beginPopUp(m_id)) {
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
    if (gui.button("..")) {
        if (m_history.size() > 1) {
            m_history.pop_back();
            m_currentSelection = m_history.back();
        } else {
            m_history.clear();

            m_currentSelection = m_currentSelection.substr(0, m_currentSelection.find_last_of("/"));
            m_history.push_back(m_currentSelection);
        }
    }

    for (auto entry = m_history.begin(); entry != m_history.end(); ++entry) {
        gui.sameLine();
        if (auto name = extractNameFromPath(*entry); gui.button(name)) {
            m_currentSelection = *entry;
            m_history = std::vector(m_history.begin(), std::next(entry));

            return;
        }
    }
}

void FileBrowser::handleFileExplorer(GuiApi& gui) {
    auto root = m_fileSystem->isDirectory(m_currentSelection) ? m_currentSelection : m_history.back();

    for (auto& entry : m_fileSystem->listDirectory(root)) {
        auto entryName = extractNameFromPath(entry);
        bool isDirectory = m_fileSystem->isDirectory(entry);

        auto entryRecord = fmt::format("  {} {}", isDirectory ? ICON_FA_FOLDER_OPEN : ICON_FA_FILE, entryName);

        if (entry == m_currentSelection) {
            gui.pushTextColor(gui::selectedEntryColor);
            gui.displayText(entryRecord);
            gui.pushTextColor(gui::guiDefaultTextColor);
        } else {
            gui.displayText(entryRecord);
        }

        if (gui.isPreviousWidgetClicked()) {
            m_currentSelection = entry;

            if (isDirectory)
                m_history.push_back(entry);
        }
    }
}

void FileBrowser::handleBottomPanel(GuiApi& gui) {
    if (gui.button(ICON_FA_CHECK_CIRCLE "  Ok")) {
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
    gui.pushItemWidth(400);
    gui.inputText("##fileSelection", m_currentSelection);

    if (m_history.size() == 0)
        m_history.push_back(m_currentSelection);

    gui.popItemWidth();
}

std::string FileBrowser::extractNameFromPath(const std::string& path) {
    const auto fileNamePosition = path.find_last_of("/\\");
    return path.substr(fileNamePosition + 1);
}

}
