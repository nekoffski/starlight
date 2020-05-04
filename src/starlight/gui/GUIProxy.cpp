#include <starlight/gui/GUIProxy.h>
#include <starlight/gui/Window.h>

#include <iostream>

namespace starl::gui {
Window GUIProxy::createWindow(std::string title, math::Vec2 pos, math::Vec2 size) {
    return Window{ std::move(title), std::move(pos), std::move(size), m_guiAdapter };
}
}