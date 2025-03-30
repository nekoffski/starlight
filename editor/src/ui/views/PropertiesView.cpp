#include "PropertiesView.hh"

#include <starlight/core/TaskQueue.hh>

namespace sle {

PropertiesView::PropertiesView(Widget::State& state) :
    Widget(state), m_tabMenu("Properties"), m_gizmoOperationCombo("Gizmo Operation"),
    m_gizmoModeCombo("Gizmo Mode") {
    m_tabMenu.addTab(ICON_FA_TOOLBOX "  Tools", [&]() { renderToolsTab(); })
      .addTab(ICON_FA_EYE "  Renderer", [&]() { renderRendererTab(); })
      .addTab(ICON_FA_CAMERA "  Camera", [&]() { renderCameraTab(); });

    m_gizmoOperationCombo.addOption("Translate", ImGuizmo::TRANSLATE)
      .addOption("Rotate", ImGuizmo::ROTATE);

    m_gizmoModeCombo.addOption("Local", ImGuizmo::LOCAL)
      .addOption("World", ImGuizmo::WORLD);
}

void PropertiesView::render() { m_tabMenu.render(); }

void PropertiesView::renderRendererTab() {
    sl::namedScope("inspector-view-renderer-tab", [&]() {
        sl::text(ICON_FA_NETWORK_WIRED "   Render graph");
        sl::separator();

        bool changed = false;

        getRenderGraph().forEach([&](auto& active, auto& renderPass) {
            if (renderPass.name != "UIRenderPass") {  // TODO: find better way
                sl::namedScope(renderPass.name, [&]() {
                    if (sl::checkbox("##Active", active)) changed = true;
                    sl::sameLine();
                    sl::text(renderPass.name);
                });
            }
        });

        if (changed) {
            sl::TaskQueue::get().callPostFrame([&renderGraph = getRenderGraph()]() {
                renderGraph.rebuildChain();
            });
        }
    });
}

void PropertiesView::renderCameraTab() {
    sl::checkbox("Center on selected entity", getState().centerOnSelectedEntity);
}

void PropertiesView::renderToolsTab() {
    if (sl::button("Full Screen Preview", sl::parentWidth)) {
    }

    sl::separator();
    sl::text("Simulation");

    if (sl::button("Play")) {
    }

    sl::sameLine();

    if (sl::button("Pause")) {
    }

    sl::sameLine();

    if (sl::button("Stop")) {
    }

    sl::separator();
    m_gizmoOperationCombo.render([&](const auto& selectedOperation) {
        getGizmoOperation() = selectedOperation;
    });
    m_gizmoModeCombo.render([&](const auto& selectedMode) {
        getGizmoMode() = selectedMode;
    });
    sl::separator();
}

}  // namespace sle
