#pragma once

#include <functional>
#include <string>

#include "GuiApi.h"
#include "sl/core/FileSystem.h"

namespace sl::gui {

class FileBrowser {
public:
    using Callback = std::function<void(const std::string&)>;

    explicit FileBrowser(const std::string& id,
        std::unique_ptr<core::FileSystem> fileSystem = std::make_unique<core::FileSystem>());

    void open(std::string* path, GuiApi& gui, std::optional<Callback> = std::nullopt);
    void show(GuiApi& gui);

private:
    void handleHistory(GuiApi& gui);
    void handleFileExplorer(GuiApi& gui);
    void handleBottomPanel(GuiApi& gui);

    std::string extractNameFromPath(const std::string& path);

    std::string m_id;
    std::string m_currentSelection = "";
    std::string* m_path;
    std::string m_root;

    std::unique_ptr<core::FileSystem> m_fileSystem;

    std::optional<Callback> m_callback;

    std::vector<std::string> m_history;
};
}
