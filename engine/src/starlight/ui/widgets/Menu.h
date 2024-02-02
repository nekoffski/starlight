#pragma once

#include <optional>
#include <vector>

#include "starlight/ui/Widget.h"
#include "starlight/core/utils/Log.h"

namespace sl::ui {

class Menu : public Widget {
public:
    struct Item {
        std::string name;
        std::optional<std::string> shortcut;
        Callback callback;
    };

    explicit Menu(const std::string& name);

    Menu& addItem(const std::string& name, Callback&& callback);
    Menu& addItem(
      const std::string& name, const std::string& shortcut, Callback&& callback
    );
    void render() override;

    const std::string& getName() const;

private:
    std::string m_name;
    std::vector<Item> m_items;
};

class MainMenuBar : public Widget {
public:
    explicit MainMenuBar();

    Menu& addMenu(const std::string& name);
    void render() override;

private:
    inline static bool sCreated = false;
    std::vector<Menu> m_menus;
};

}  // namespace sl::ui
