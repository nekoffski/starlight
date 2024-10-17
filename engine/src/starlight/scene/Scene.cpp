#include "Scene.hh"

namespace sl {

Scene::Scene(Window& window, Camera* camera) : m_window(window), m_camera(camera) {}

RenderPacket Scene::getRenderPacket() {
    RenderPacket packet{};
    packet.camera = m_camera;

    return packet;
}

Entity& Scene::addEntity(const std::string& name) {}

Entity& Scene::addEntity() {}

}  // namespace sl
