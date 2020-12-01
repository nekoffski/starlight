#include "GUIProxy.h"

#include "sl/gui/Panel.h"
#include "sl/misc/mem.hpp"

namespace sl::gui {
std::shared_ptr<Panel> GUIProxy::createPanel(std::string title, math::Vec2 pos, math::Vec2 size) {
    return std::shared_ptr<Panel>(new Panel(std::move(title), std::move(pos), std::move(size), m_guiImpl));
}
}
