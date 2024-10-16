#pragma once

#include "starlight/core/Core.hh"
#include "starlight/core/window/Window.hh"
#include "starlight/renderer/camera/Camera.hh"
#include "starlight/renderer/RenderPacket.hh"

namespace sl {

class Scene {
public:
    explicit Scene(Window& window, Camera* camera);

    RenderPacket getRenderPacket();

private:
    Window& m_window;
    Camera* m_camera;
};

}  // namespace sl
