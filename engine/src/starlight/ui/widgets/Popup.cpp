#include "Popup.h"

namespace sl::ui {

Popup::Popup(const std::string& name, Callback&& callback) :
    m_name(name), m_callback(std::move(callback)) {}

void Popup::render() {
    if (ImGui::BeginPopupModal(m_name.c_str())) {
        m_callback();
        ImGui::EndPopup();
    }
}

void Popup::open() { ImGui::OpenPopup(m_name.c_str()); }

}  // namespace sl::ui
