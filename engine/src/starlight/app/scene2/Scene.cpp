#include "Scene.hh"

namespace sl::v2 {

RenderPacket Scene::assembleRenderPacket() {
    RenderPacket packet;

    packet.skybox = m_skybox.get();

    return packet;
}

Entity& Scene::addEntity(OptStr name) { return m_entityManager.addEntity(name); }

bool Scene::hasEntity(const std::string& name) {
    return m_entityManager.hasEntity(name);
}

Entity* Scene::getEntity(const std::string& name) {
    return m_entityManager.getEntity(name);
}

void Scene::setSkybox(SharedPtr<Skybox> skybox) { m_skybox = std::move(skybox); }
void Scene::resetSkybox() { m_skybox.reset(); }

}  // namespace sl::v2
