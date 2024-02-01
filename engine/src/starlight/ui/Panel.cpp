#include "Panel.h"

#include <imgui.h>

namespace sl::ui {

Panel::Panel(const std::string& name, const Properties& props, Callback&& callback) :
    m_name(name), m_props(props), m_callback(std::move(callback)) {}

void Panel::render() {
    ImGui::SetNextWindowPos({ m_props.position.x, m_props.position.y });
    ImGui::SetNextWindowSize({ m_props.size.x, m_props.size.y });

    if (ImGui::Begin(m_name.c_str())) {
        m_callback();
        ImGui::End();
    }
}

void Panel::setSize(const Vec2f& size) { m_props.size = size; }
void Panel::setPosition(const Vec2f& position) { m_props.position = position; }

PanelCombo::PanelCombo(const std::string& name, const Properties& props) :
    m_name(name), m_props(props) {}

void PanelCombo::render() {
    for (auto& panel : m_panels) panel.render();
}

PanelCombo& PanelCombo::addPanel(const std::string& name, Callback&& callback) {
    m_panels.emplace_back(name, m_props, std::move(callback));
    rebuildPanels();
    return *this;
}

void PanelCombo::setSize(const Vec2f& size) {
    m_props.size = size;
    rebuildPanels();
}
void PanelCombo::setPosition(const Vec2f& position) {
    m_props.position = position;
    rebuildPanels();
}

void PanelCombo::rebuildPanels() {
    const auto panelCount   = m_panels.size();
    const float panelHeight = m_props.size.y / panelCount;

    for (int i = 0; i < panelCount; ++i) {
        auto panel = &m_panels[i];

        panel->setPosition(
          { m_props.position.x, m_props.position.y + i * panelHeight }
        );
        panel->setSize({ m_props.size.x, panelHeight });
    }
}

}  // namespace sl::ui
