#pragma once

#include "EulerCamera.hh"
#include "starlight/window/WindowTypes.hh"

namespace sl {

class Window;

class FlyCameraController {
   public:
    Result<void> update(
        EulerCamera& camera, const Window& window, f32 deltaSeconds
    );

   private:
    Vec2f mouseLookDelta(const Window& window);

    WindowId m_window{0};
    Opt<Vec2f> m_previousMousePosition;
};

}  // namespace sl
