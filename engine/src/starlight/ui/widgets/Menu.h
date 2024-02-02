#pragma once

#include <unordered_map>
#include <optional>

#include "starlight/ui/Widget.h"
#include "starlight/core/utils/Log.h"

namespace sl::ui {

class Menu : public Widget {
public:
    struct Item {
        std::optional<std::string> shortcut;
        Callback callback;
    };

    explicit Menu(const std::string& name) : m_name(name) {}

    Menu& addItem(const std::string& name, Callback&& callback) {
        m_items[name] = Item{ {}, std::move(callback) };
        return *this;
    }

    Menu& addItem(
      const std::string& name, const std::string& shortcut, Callback&& callback
    ) {
        m_items[name] = Item{ shortcut, std::move(callback) };
        return *this;
    }

    void render() override {
        if (ImGui::BeginMenu(m_name.c_str())) {
            for (auto& [name, item] : m_items) {
                if (item.shortcut) {
                    if (ImGui::MenuItem(name.c_str(), item.shortcut->c_str()))
                        item.callback();
                } else {
                    if (ImGui::MenuItem(name.c_str())) item.callback();
                }
            }
            ImGui::EndMainMenuBar();
        }
    }

private:
    std::string m_name;
    std::unordered_map<std::string, Item> m_items;
};

class MainMenuBar : public Widget {
public:
    explicit MainMenuBar() {
        ASSERT(not sCreated, "Only 1 instance of MainMenuBar is allowed!");
        sCreated = true;
    }

    Menu& addMenu(const std::string& name) {
        auto [iterator, inserted] = m_menus.insert({ name, Menu{ name } });
        ASSERT(inserted, "Menu '{}' already added to MainMenuBar", name);
        return iterator->second;
    }

    void render() override {
        if (ImGui::BeginMainMenuBar()) {
            for (auto& menu : m_menus | std::views::values) menu.render();
            ImGui::EndMainMenuBar();
        }
    }

private:
    inline static bool sCreated = false;
    std::unordered_map<std::string, Menu> m_menus;
};

}  // namespace sl::ui
