#include "Scene.hh"

#include "starlight/renderer/MeshTree.hh"

namespace sl {

Scene::Scene(Window& window, Camera* camera) : m_window(window), m_camera(camera) {}

RenderPacket Scene::getRenderPacket() {
    RenderPacket packet{};
    packet.camera = m_camera;

    m_componentManager.getComponentContainer<MeshTree>().forEach(
      [&](Component<MeshTree>& meshTree) {
          packet.entities.emplace_back(
            sl::math::scale(
              sl::identityMatrix, sl::Vec3<sl::f32>{ 0.25f, 0.25f, 0.25f }
            ),
            meshTree.data().mesh, meshTree.data().material
          );
      }
    );

    return packet;
}

Entity& Scene::addEntity(std::optional<std::string> name) {
    m_entities.emplace_back(m_componentManager, name);
    return m_entities.back();
}

}  // namespace sl
