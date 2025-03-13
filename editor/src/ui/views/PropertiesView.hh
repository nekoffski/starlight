#pragma once

#include <starlight/ui/UI.hh>

#include "Console.hh"
#include "ui/Data.hh"

namespace sle {

class PropertiesView {
public:
    explicit PropertiesView(Data& data);
    void render();

private:
    Data& m_data;
    sl::ui::TabMenu m_tabMenu;

    void renderInspectorTab();
    void renderRendererTab();
};

}  // namespace sle
