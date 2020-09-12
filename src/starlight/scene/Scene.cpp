#include "Scene.h"

#include <memory>
#include <unordered_map>
#include <vector>

#include "starlight/ecs/Entity.h"

#include "starlight/scene/components/DirectionalLightComponentWrapper.h"
#include "starlight/scene/components/MaterialComponentWrapper.h"
#include "starlight/scene/components/PFXComponentWrapper.h"
#include "starlight/scene/components/PointLightComponentWrapper.h"
#include "starlight/scene/components/TransformComponentWrapper.h"

namespace sl::scene {

std::shared_ptr<Scene> Scene::create() {
    auto scene = std::make_shared<Scene>();
    scene->setComponentWrapperFactory<components::TransformComponent,
        components::TransformComponentWrapperFactory>();
    scene->setComponentWrapperFactory<components::MaterialComponent,
        components::MaterialComponentWrapperFactory>();
    scene->setComponentWrapperFactory<components::DirectionalLightComponent,
        components::DirectionalLightComponentWrapperFactory>();
    scene->setComponentWrapperFactory<components::PointLightComponent,
        components::PointLightComponentWrapperFactory>();
    scene->setComponentWrapperFactory<components::PFXComponent,
        components::PFXComponentWrapperFactory>();
    return scene;
}

void Scene::setSkybox(std::shared_ptr<Skybox> skybox) {
    m_skybox = std::move(skybox);
}

std::shared_ptr<ecs::Entity> Scene::addEntity(std::string name) {
    return m_ecsRegistry.createEntity(std::move(name));
}
}
