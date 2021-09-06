#pragma once

#include <optional>

#include "GuiApi.h"

namespace sl::gui {

const std::string id = "ErrorPopUp";

class ErrorDialog {
public:
    void setErrorMessage(const std::string& errorMessage) {
        m_errorMessage = errorMessage;
    }

    void show(GuiApi& gui) {
        gui.pushId(m_id);

        if (m_errorMessage.has_value() && not m_showed) {
            LOG_DEBUG("Opening error dialog {} with message: {}", m_id, m_errorMessage.value());
            m_showed = true;
            gui.openPopUp(m_id);
        }

        if (gui.beginPopUp(m_id)) {
            gui.displayText(m_errorMessage.value());

            if (gui.button("OK")) {
                m_errorMessage = std::nullopt;
                m_showed = false;

                LOG_DEBUG("Closing error dialog");
                gui.closeCurrentPopUp();
            }

            gui.endPopUp();
        }

        gui.popId();
    }

private:
    inline static int idGenerator = 0;

    std::string m_id = id + std::to_string(idGenerator++);
    bool m_showed = false;
    std::optional<std::string> m_errorMessage = std::nullopt;
};
}
