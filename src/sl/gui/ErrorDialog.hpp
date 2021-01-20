#pragma once

#include <optional>

#include "GuiApi.h"

namespace sl::gui {

const std::string id = "ErrorPopUp";

class ErrorDialog {
public:
    void setErrorMessage(const std::string& errorMessage, GuiApi& gui) {
        m_errorMessage = errorMessage;
        gui.openPopUp(id);
    }

    void show(GuiApi& gui) {
        gui.beginGroup();
        if (gui.beginPopUp(id)) {
            gui.displayText(m_errorMessage.value());

            if (gui.button("OK")) {
                m_errorMessage = std::nullopt;
				gui.closeCurrentPopUp();
            }

            gui.endPopUp();
        }
        gui.endGroup();
    }

private:
    std::optional<std::string> m_errorMessage = std::nullopt;
};
}
