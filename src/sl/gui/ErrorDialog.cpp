#include "ErrorDialog.h"

#include <imgui_sugar.hpp>
#include <kc/core/Log.h>

namespace sl::gui {

static std::string generateId() {
    static const std::string idBase = "ErrorPopUp";
    static int intGenerator = 0;

    return idBase + std::to_string(intGenerator++);
}

ErrorDialog::ErrorDialog()
    : m_showed(false)
    , m_id(generateId()) {
}

void ErrorDialog::setErrorMessage(const std::string& errorMessage) {
    m_errorMessage = errorMessage;
}

void ErrorDialog::show() {
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

}
