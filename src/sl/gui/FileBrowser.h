#pragma once

#include <functional>
#include <string>

#include <kc/core/FileSystem.h>

#include "GuiApi.h"

namespace sl::gui {

class FileBrowser {
public:
    using Callback = std::function<void(const std::string&)>;

    explicit FileBrowser(const std::string& id,
        std::unique_ptr<kc::core::FileSystem> fileSystem = std::make_unique<kc::core::FileSystem>());

    void open(Callback&&);
    void show();

private:
    void handleHistory();
    void handleFileExplorer();
    void handleBottomPanel();

    std::string extractNameFromPath(const std::string& path);

    std::string m_id;
    std::string m_currentSelection = "";

    std::unique_ptr<kc::core::FileSystem> m_fileSystem;

    std::optional<Callback> m_callback;

    std::vector<std::string> m_history;
};
}
