#pragma once

#include <string>

#include "GuiApi.h"
#include "sl/core/FileSystem.h"

namespace sl::gui {

class FileBrowser {
public:
    explicit FileBrowser(const std::string& id)
        : m_id(id) {}

    void open(std::string* path, GuiApi& gui) {
        m_path = path;
        gui.openPopUp(m_id);
    }

    void show(GuiApi& gui) {
        gui.setNextWindowSize({ 700.0f, 450.0f });
        if (gui.beginPopUp(m_id)) {
            const std::string root = "/home/neko/kapik/projects/starlight";

            if (gui.button("Quit"))
                gui.closeCurrentPopUp();

            gui.sameLine();
            if (gui.button("Pick")) {
                *m_path = m_currentSelection;
                gui.closeCurrentPopUp();
            }

            gui.breakLine();
            gui.displayText(m_currentSelection);
            gui.displayText("\n");

            gui.beginChild("##fileBrowserChild");
            processDirectory(root, gui);
            gui.endChild();

            gui.endPopUp();
        }
    }

private:
    void processDirectory(const std::string& root, GuiApi& gui) {
        for (auto& entry : core::FS::listDirectory(root)) {
            auto entryName = extractNameFromPath(entry);
            if (core::FS::isDirectory(entry)) {
                if (gui.beginTreeNode(entryName, false)) {
                    processDirectory(entry, gui);
                    gui.popTreeNode();
                }
            } else {
                gui.displayText(entryName);
                if (gui.isPreviousWidgetClicked())
                    m_currentSelection = entry;
            }
        }
    }

    std::string extractNameFromPath(const std::string& path) {
        const auto fileNamePosition = path.find_last_of("/\\");
        return path.substr(fileNamePosition + 1);
    }

    std::string m_id;
    std::string m_currentSelection = "";
    std::string* m_path;
};
}
