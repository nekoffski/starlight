#pragma once

#include <vector>

#include "starlight/ui/Core.h"

namespace sl::ui {

class TabMenu {
    struct Tab {
        std::string name;
        Callback callback;
    };

public:
    explicit TabMenu(const std::string& name);

    void render();
    TabMenu& addTab(const std::string& name, Callback&& callback);

private:
    std::string m_name;
    std::vector<Tab> m_tabs;
};

}  // namespace sl::ui