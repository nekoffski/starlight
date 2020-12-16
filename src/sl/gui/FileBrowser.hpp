#pragma once

#include <string>

#include "GuiApi.h"
#include "sl/core/FileSystem.h"

namespace sl::gui {

class FileBrowser {
public:
    explicit FileBrowser(GuiApi& gui, std::string id)
        : m_gui(gui)
        , m_id(std::move(id))
        , m_path(nullptr) {} // TODO: add uuid here

    void open(std::string* path) {
        m_path = path;
        m_gui.openPopUp(m_id);
    }

    void show() {
        if (m_gui.beginPopUp(m_id)) {
            const std::string root = "/home/neko/kapik/projects/starlight";
            processDirectory(root);

            m_gui.breakLine();
            m_gui.displayText(*m_path);

            if (m_gui.button("Pick"))
                m_gui.closeCurrentPopUp();

            m_gui.endPopUp();
        }
    }

private:
    void processDirectory(const std::string& root) {
        for (auto& entry : core::FS::listDirectory(root)) {
            auto entryName = extractNameFromPath(entry);
            if (core::FS::isDirectory(entry)) {
                if (m_gui.beginTreeNode(entryName, false)) {
                    processDirectory(entry);
                    m_gui.popTreeNode();
                }
            } else {
                m_gui.displayText(entryName);
                if (m_gui.isPreviousWidgetClicked())
                    *m_path = entry;
            }
        }
    }

    std::string extractNameFromPath(const std::string& path) {
        const auto fileNamePosition = path.find_last_of("/\\");
        return path.substr(fileNamePosition + 1);
    }

    GuiApi& m_gui;
    std::string m_id;
    std::string* m_path;
};
}
