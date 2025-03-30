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
    sl::TabMenu m_tabMenu;

    void renderRendererTab();
    void renderCameraTab();
    void renderToolsTab();

    sl::Combo<ImGuizmo::OPERATION> m_gizmoOperationCombo;
    sl::Combo<ImGuizmo::MODE> m_gizmoModeCombo;
};

}  // namespace sle
