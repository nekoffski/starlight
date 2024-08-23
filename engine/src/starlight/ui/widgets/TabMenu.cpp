#include "TabMenu.hh"

namespace sl::ui {

TabMenu::TabMenu(const std::string& name) : m_name(name) {}

void TabMenu::render() {
    if (ImGui::BeginTabBar(m_name.c_str())) {
        for (auto& [name, callback] : m_tabs) {
            if (ImGui::BeginTabItem(name.c_str())) {
                callback();
                ImGui::EndTabItem();
            }
        }
        ImGui::EndTabBar();
    }
}

TabMenu& TabMenu::addTab(const std::string& name, Callback&& callback) {
    m_tabs.emplace_back(name, std::move(callback));
    return *this;
}

}  // namespace sl::ui
