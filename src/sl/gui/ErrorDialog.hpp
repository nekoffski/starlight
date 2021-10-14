#pragma once

#include <optional>

#include <imgui_sugar.hpp>

namespace sl::gui {

const std::string id = "ErrorPopUp";

class ErrorDialog {
public:
    void setErrorMessage(const std::string& errorMessage) {
        m_errorMessage = errorMessage;
    }

    void show() {
        with_ID(m_id.c_str()) {

            if (m_errorMessage.has_value() && not m_showed) {
                LOG_DEBUG("Opening error dialog {} with message: {}", m_id, m_errorMessage.value());
                m_showed = true;
                ImGui::OpenPopup(m_id.c_str());
            }

            with_Popup(m_id.c_str()) {
                ImGui::Text("%s", m_errorMessage.value().c_str());

                if (ImGui::Button("OK")) {
                    m_errorMessage = std::nullopt;
                    m_showed = false;

                    LOG_DEBUG("Closing error dialog");
                    ImGui::CloseCurrentPopup();
                }
            }
        }
    }

private:
    inline static int idGenerator = 0;

    std::string m_id = id + std::to_string(idGenerator++);
    bool m_showed = false;
    std::optional<std::string> m_errorMessage = std::nullopt;
};
}
