#pragma once

#include <memory>

#include <starlight/math/Vector.hpp>
#include <starlight/misc/types/NotNullPtr.hpp>

namespace starl::platform::gui::impl {

class GUIImpl {
public:
    static std::unique_ptr<GUIImpl> create(void*);
    virtual ~GUIImpl();

    virtual void begin() = 0;
    virtual void end() = 0;

    virtual void beginWindow(std::string, math::Vec2, math::Vec2) = 0;
    virtual void endWindow() = 0;
};
}