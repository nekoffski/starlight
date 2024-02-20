#pragma once

#include <optional>
#include <vector>

#include "starlight/ui/Core.h"
#include "starlight/core/utils/Log.h"

namespace sl::ui {

class Popup {
public:
    explicit Popup(const std::string& name, Callback&& callback);

    void open();
    void render();

private:
    std::string m_name;
    Callback m_callback;
};

}  // namespace sl::ui
