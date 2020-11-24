#include "GUIProxy.h"

#include "sl/gui/Window.h"

namespace sl::gui {
Window GUIProxy::createWindow(std::string title, math::Vec2 pos, math::Vec2 size) {
    return Window{ std::move(title), std::move(pos), std::move(size), m_guiImpl };
}
}
