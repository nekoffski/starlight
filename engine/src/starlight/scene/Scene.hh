#pragma once

#include "starlight/core/Core.hh"
#include "starlight/core/window/Window.hh"
#include "starlight/renderer/camera/Camera.hh"
#include "starlight/renderer/RenderPacket.hh"

#include "Entity.hh"

namespace sl {

class Scene {
public:
    explicit Scene(Window& window, Camera* camera);

    RenderPacket getRenderPacket();

    Entity& addEntity(const std::string& name);
    Entity& addEntity();

private:
    Window& m_window;
    Camera* m_camera;
};

}  // namespace sl
