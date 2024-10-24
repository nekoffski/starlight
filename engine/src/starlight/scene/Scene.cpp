#include "Scene.hh"

#include "starlight/renderer/MeshComposite.hh"

namespace sl {

Scene::Scene(Window& window, Camera* camera) : m_window(window), m_camera(camera) {}

RenderPacket Scene::getRenderPacket() {
    RenderPacket packet{};
    packet.camera = m_camera;

    m_componentManager.getComponentContainer<MeshComposite>().forEach(
      [&](Component<MeshComposite>& meshComposite) {
          meshComposite.data().traverse([&](MeshComposite::Node& node) {
              for (auto& instance : node.getInstances()) {
                  packet.entities.emplace_back(
                    instance.getWorld(), node.getMesh(), node.getMaterial()
                  );
              }
          });
      }
    );

    return packet;
}

Entity& Scene::addEntity(std::optional<std::string> name) {
    m_entities.emplace_back(m_componentManager, name);
    return m_entities.back();
}

}  // namespace sl
