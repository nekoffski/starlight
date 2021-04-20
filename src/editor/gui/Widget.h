#pragma once

#include <memory>

#include "GuiStyle.h"
#include "SharedState.hpp"
#include "sl/gui/GuiApi.h"
#include "sl/gui/fonts/FontAwesome.h"
#include "sl/math/Vector.hpp"
#include "sl/scene/Scene.h"

namespace editor::gui {

class Widget {
public:
    explicit Widget(std::shared_ptr<SharedState> sharedState)
        : m_sharedState(sharedState) {
    }

    virtual void render(sl::gui::GuiApi& gui) = 0;

protected:
    std::shared_ptr<SharedState> m_sharedState;
};

struct WidgetPosition {
    sl::math::Vec2 origin;
    sl::math::Vec2 size;
};
}
