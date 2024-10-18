#include "Scene.hh"

namespace sl {

Scene::Scene(Window& window, Camera* camera) : m_window(window), m_camera(camera) {}

RenderPacket Scene::getRenderPacket() {
    RenderPacket packet{};
    packet.camera = m_camera;

    return packet;
}

Entity& Scene::addEntity(std::optional<std::string> name) {
    m_entities.emplace_back(m_componentManager, name);
    return m_entities.back();
}

}  // namespace sl
