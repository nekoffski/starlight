#pragma once

#include <memory>

#include "SharedState.hpp"
#include "sl/core/Errors.hpp"

#include "sl/gui/GuiStyle.h"
#include "sl/gui/fonts/FontAwesome.h"
#include "sl/math/Vector.hpp"
#include "sl/scene/Scene.h"

namespace editor::gui {

class Widget {
public:
    explicit Widget(std::shared_ptr<SharedState> sharedState)
        : m_sharedState(sharedState) {
    }

    virtual void render() = 0;

protected:
    void raise(const std::string& reason) {
        throw sl::core::GuiError { reason };
    }

    std::shared_ptr<SharedState> m_sharedState;
};

struct WidgetPosition {
    sl::math::Vec2 origin;
    sl::math::Vec2 size;
};
}
