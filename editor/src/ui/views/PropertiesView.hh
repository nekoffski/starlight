#pragma once

#include <starlight/ui/UI.hh>

#include "Console.hh"
#include "ui/Widget.hh"

namespace sle {

class PropertiesView : public Widget {
public:
    explicit PropertiesView(Widget::State& state);
    void render();

private:
    sl::ui::TabMenu m_tabMenu;

    void renderInspectorTab();
    void renderRendererTab();
};

}  // namespace sle
