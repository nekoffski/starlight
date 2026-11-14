#pragma once

#include <kcore/Concepts.hh>

#include "WindowTypes.hh"
#include "starlight/math/Math.hh"

namespace sl {

class Window : public virtual kc::NonCopyable, public virtual kc::NonMovable {
   public:
    virtual ~Window() = default;

    // TODO: Centralize event pumping; SDL's queue serves all windows, so poll
    // once per engine-loop iteration through any live window.
    virtual void pollEvents() = 0;
    virtual WindowId id() const = 0;

    // Current native state; use window events for transitions and scrolling.
    virtual bool isPressed(Key key) const = 0;

    virtual bool isPressed(MouseButton button) const = 0;

    virtual bool isFocused() const = 0;
    // Window-relative coordinates, or zero when the window lacks mouse focus.
    virtual Vec2f mousePosition() const = 0;
};

}  // namespace sl
