#include "PropertiesView.hh"

#include <starlight/core/TaskQueue.hh>

namespace sle {

PropertiesView::PropertiesView(Data& data) : m_data(data), m_tabMenu("Properties") {
    m_tabMenu.addTab(ICON_FA_WRENCH "  Inspector", [&]() { renderInspectorTab(); })
      .addTab(ICON_FA_EYE "  Renderer", [&]() { renderRendererTab(); });
}

void PropertiesView::render() { m_tabMenu.render(); }

void PropertiesView::renderRendererTab() {
    sl::ui::namedScope("inspector-view-renderer-tab", [&]() {
        sl::ui::text(ICON_FA_NETWORK_WIRED "   Render graph");
        sl::ui::separator();

        bool changed = false;

        m_data.renderGraph->forEach([&](auto& active, auto& renderPass) {
            if (renderPass.name != "UIRenderPass") {  // TODO: find better way
                sl::ui::namedScope(renderPass.name, [&]() {
                    if (sl::ui::checkbox("##Active", active)) changed = true;
                    sl::ui::sameLine();
                    sl::ui::text(renderPass.name);
                });
            }
        });

        if (changed) {
            sl::TaskQueue::get().callPostFrame([&]() {
                m_data.renderGraph->rebuildChain();
            });
        }
    });
}

void PropertiesView::renderInspectorTab() {
    static auto defaultPanel = []() { sl::ui::text("Nothing to show"); };
    std::invoke(m_data.inspectorCallback.value_or(defaultPanel));
}

}  // namespace sle
