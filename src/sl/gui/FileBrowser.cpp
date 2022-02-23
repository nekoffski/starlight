#include "FileBrowser.h"

#include <kc/core/Log.h>

#include <imgui_sugar.hpp>

#include "GuiStyle.h"
#include "Utils.h"
#include "fonts/FontAwesome.h"

namespace sl::gui {

FileBrowser::FileBrowser(const std::string& id, std::unique_ptr<kc::core::FileSystem> fileSystem)
    : m_id(id), m_fileSystem(std::move(fileSystem)) {}

void FileBrowser::open(Callback&& callback) {
    static const std::string rootPath =
        "/home/nek0/kapik/projects/starlight";  // TODO: get current directory
    m_currentSelection = rootPath;

    m_history.clear();
    m_history.push_back(m_currentSelection);

    m_callback = callback;

    LOG_TRACE("Opening file browser");
    ImGui::OpenPopup(m_id.c_str());
}

void FileBrowser::show() {
    constexpr float errorDialogWidth = 700.0f;
    ImGui::SetNextWindowSize(ImVec2(errorDialogWidth, 0.0f));

    with_Popup(m_id.c_str()) {
        handleHistory();
        ImGui::Separator();

        constexpr float mainContentRatio = 0.6f;
        const float mainContentHeight = 450.0f * mainContentRatio;

        with_Child("##fileBrowserChild", ImVec2(0.0f, mainContentHeight)) handleFileExplorer();

        ImGui::Separator();
        handleBottomPanel();
    }
}

void FileBrowser::handleHistory() {
    if (ImGui::Button("..")) {
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
        ImGui::SameLine();
        if (auto name = extractNameFromPath(*entry); ImGui::Button(name.c_str())) {
            m_currentSelection = *entry;
            m_history = std::vector(m_history.begin(), std::next(entry));

            return;
        }
    }
}

void FileBrowser::handleFileExplorer() {
    auto root =
        m_fileSystem->isDirectory(m_currentSelection) ? m_currentSelection : m_history.back();

    for (auto& entry : m_fileSystem->listDirectory(root)) {
        auto entryName = extractNameFromPath(entry);
        bool isDirectory = m_fileSystem->isDirectory(entry);

        auto entryRecord =
            fmt::format("  {} {}", isDirectory ? ICON_FA_FOLDER_OPEN : ICON_FA_FILE, entryName);

        if (entry == m_currentSelection) {
            gui::pushTextColor(gui::selectedEntryColor);
            ImGui::Text("%s", entryRecord.c_str());
            gui::popTextColor();
        } else {
            ImGui::Text("%s", entryRecord.c_str());
        }

        if (ImGui::IsItemClicked()) {
            m_currentSelection = entry;

            if (isDirectory) m_history.push_back(entry);
        }
    }
}

void FileBrowser::handleBottomPanel() {
    if (ImGui::Button(ICON_FA_CHECK_CIRCLE "  Ok")) {
        if (m_callback.has_value()) std::invoke(m_callback.value(), m_currentSelection);

        ImGui::CloseCurrentPopup();
    }

    ImGui::SameLine();

    if (ImGui::Button(ICON_FA_TIMES_CIRCLE "  Cancel")) ImGui::CloseCurrentPopup();

    ImGui::SameLine();
    ImGui::Text("Full path: ");
    ImGui::SameLine();
    ImGui::PushItemWidth(400);
    gui::textInput("##fileSelection", m_currentSelection);

    if (m_history.empty()) m_history.push_back(m_currentSelection);

    ImGui::PopItemWidth();
}

// TODO: extract to libkc
std::string FileBrowser::extractNameFromPath(const std::string& path) {
    const auto fileNamePosition = path.find_last_of("/\\");
    return path.substr(fileNamePosition + 1);
}

}  // namespace sl::gui
