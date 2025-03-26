#include "PropertiesView.hh"

#include <starlight/core/TaskQueue.hh>

namespace sle {

PropertiesView::PropertiesView(Widget::State& state
) : Widget(state), m_tabMenu("Properties") {
    m_tabMenu.addTab(ICON_FA_WRENCH "  Inspector", [&]() { renderInspectorTab(); })
      .addTab(ICON_FA_TOOLBOX "  Tools", [&]() { renderToolsTab(); })
      .addTab(ICON_FA_EYE "  Renderer", [&]() { renderRendererTab(); })
      .addTab(ICON_FA_CAMERA "  Camera", [&]() { renderCameraTab(); });
}

void PropertiesView::render() { m_tabMenu.render(); }

void PropertiesView::renderRendererTab() {
    sl::ui::namedScope("inspector-view-renderer-tab", [&]() {
        sl::ui::text(ICON_FA_NETWORK_WIRED "   Render graph");
        sl::ui::separator();

        bool changed = false;

        getRenderGraph().forEach([&](auto& active, auto& renderPass) {
            if (renderPass.name != "UIRenderPass") {  // TODO: find better way
                sl::ui::namedScope(renderPass.name, [&]() {
                    if (sl::ui::checkbox("##Active", active)) changed = true;
                    sl::ui::sameLine();
                    sl::ui::text(renderPass.name);
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

void PropertiesView::renderInspectorTab() {
    static auto defaultPanel = []() { sl::ui::text("Nothing to show"); };
    std::invoke(getInspectorCallback().value_or(defaultPanel));
}

void PropertiesView::renderCameraTab() {
    sl::ui::checkbox("Center on selected entity", getState().centerOnSelectedEntity);
}

void PropertiesView::renderToolsTab() { sl::ui::separator(); }

}  // namespace sle
