#include "Scene.hh"

#include "starlight/app/model/Model.hh"
#include "Components.hh"

namespace sl {

static constexpr u32 maxPointLights       = 5;
static constexpr u32 maxDirectionalLights = 5;

Scene::Scene() : m_skybox(nullptr), m_entities(maxEntities) {}

RenderPacket Scene::getRenderPacket() {
    RenderPacket packet{};

    packet.directionalLights.reserve(maxDirectionalLights);
    packet.pointLights.reserve(maxPointLights);

    forEach<ModelComponent>([&](auto& c) {
        auto& model = c.data();
        model.traverse([&](Model::Sub& sub) {
            packet.entities.emplace_back(
              sub.transform.getWorld(), sub.mesh.get(), sub.material.get()
            );
        });
    });

    forEach<PointLightComponent>([&](auto& light) {
        packet.pointLights.push_back(light.data());
    });

    forEach<DirectionalLightComponent>([&](auto& light) {
        packet.directionalLights.push_back(light.data());
    });

    // mock for testing
    // PointLight light;

    // light.data.position.x = 1.0f;
    // light.data.color      = Vec4<f32>{ 1.0f, 0.5f, 0.3f, 1.0f };
    // packet.pointLights.push_back(light);

    // light.data.position.x = 0.0f;
    // light.data.position.y = 1.0f;
    // light.data.position.z = 1.0f;
    // light.data.color      = Vec4<f32>{ 0.5f, 0.5f, 0.1f, 1.0f };
    // packet.pointLights.push_back(light);

    packet.skybox = m_skybox.get();

    return packet;
}

void Scene::clear() {
    m_skybox.reset();
    m_entities.clear();
}

Entity& Scene::addEntity(std::optional<std::string> name) {
    if (name.has_value()) {
        log::expect(
          not m_entities.has([&](auto& entity) { return entity.name == *name; }),
          "Entity {} already exists", *name
        );
    }
    auto record = m_entities.emplace(m_componentManager, name);
    log::expect(record, "Could not add entity");
    return *record;
}

Entity* Scene::getEntity(const std::string& name) {
    return m_entities.find([&](auto& entity) -> bool {
        return entity.name == name;
    });
}

void Scene::setSkybox(SharedPtr<Skybox> skybox) { m_skybox = std::move(skybox); }

void Scene::resetSkybox() { m_skybox.reset(); }

Skybox* Scene::getSkybox() { return m_skybox.get(); }

}  // namespace sl
