#pragma once

#include <optional>
#include <vector>

#include "starlight/ui/Core.hh"
#include "starlight/core/Log.hh"

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
