#pragma once

#include <memory>

#include "sl/gui/GuiApi.h"
#include "sl/math/Vector.hpp"
#include "sl/scene/Scene.h"

namespace editor::gui {

class Widget {
public:
    virtual void render(sl::gui::GuiApi& gui) = 0;

    virtual void setActiveScene(std::shared_ptr<sl::scene::Scene> scene) {
        m_activeScene = scene;
    }

protected:
    std::weak_ptr<sl::scene::Scene> m_activeScene;
};

struct WidgetPosition {
    sl::math::Vec2 origin;
    sl::math::Vec2 size;
};
}
