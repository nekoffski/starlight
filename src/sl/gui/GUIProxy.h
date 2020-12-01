#pragma once

#include <memory>

#include "sl/math/Vector.hpp"
#include "sl/platform/gui/GUIImpl.h"



namespace sl::gui {

class Panel;

class GUIProxy {
public:
    GUIProxy(std::shared_ptr<platform::gui::GUIImpl> guiImpl)
        : m_guiImpl(std::move(guiImpl)) {
    }
	
	std::shared_ptr<platform::gui::GUIImpl> operator->() {
		return m_guiImpl;
	}

	std::shared_ptr<Panel> createPanel(std::string, math::Vec2, math::Vec2);

private:
    std::shared_ptr<platform::gui::GUIImpl> m_guiImpl;
};

}
