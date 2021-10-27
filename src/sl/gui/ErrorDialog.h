#pragma once

#include <optional>
#include <string>

namespace sl::gui {

class ErrorDialog {
public:
    explicit ErrorDialog();

    void setErrorMessage(const std::string& errorMessage);
    void show();

private:
    bool m_showed;
    std::string m_id;
    std::optional<std::string> m_errorMessage;
};
}
