#include "Scene.hh"

#include "starlight/app/factories/MaterialFactory.hh"

#include "Components.hh"

namespace sl {

static constexpr u32 maxPointLights       = 5;
static constexpr u32 maxDirectionalLights = 5;

Scene::Scene()
    : m_skybox(nullptr) {}

RenderPacket Scene::getRenderPacket() {
    RenderPacket packet{};

    auto defaultMaterial = MaterialFactory::get().getDefault();

    packet.directionalLights.reserve(maxDirectionalLights);
    packet.pointLights.reserve(maxPointLights);

    forEach<MeshRendererComponent>([&](auto& c) {
        packet.entities
          .emplace_back(c->getWorld(), c->mesh.get(), c->material.get());
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

    packet.skybox = m_skybox ? m_skybox.get() : nullptr;
    return packet;
}

void Scene::clear() {
    m_skybox.clear();
    m_entities.clear();
}

Entity& Scene::addEntity(std::optional<std::string> name) {
    if (name.has_value()) {
        log::expect(
          not m_entities.findIf([&](auto& entity) {
              return entity.getName() == *name;
          }),
          "Entity {} already exists", *name
        );
    }
    auto record = m_entities.emplace(m_componentManager, name);
    log::expect(record, "Could not add entity");
    return *record;
}

Entity* Scene::getEntity(const std::string& name) {
    return m_entities.findIf([&](auto& entity) -> bool {
        return entity.getName() == name;
    });
}

void Scene::setSkybox(kstd::SharedPtr<Skybox> skybox) {
    m_skybox = std::move(skybox);
}

void Scene::resetSkybox() { m_skybox.clear(); }

Skybox* Scene::getSkybox() { return m_skybox.get(); }

}  // namespace sl
