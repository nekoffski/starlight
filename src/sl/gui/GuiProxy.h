#pragma once

#include <memory>

#include "sl/math/Vector.hpp"
#include "sl/platform/gui/GuiImpl.h"

namespace sl::gui {

class Panel;

class GuiProxy {
public:
    GuiProxy(std::shared_ptr<platform::gui::GuiImpl> guiImpl)
        : m_guiImpl(std::move(guiImpl)) {
    }

    std::shared_ptr<platform::gui::GuiImpl> operator->() {
        return m_guiImpl;
    }

private:
    std::shared_ptr<platform::gui::GuiImpl> m_guiImpl;
};
}
