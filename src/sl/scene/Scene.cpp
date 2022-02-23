#include "Scene.h"

#include <memory>
#include <unordered_map>
#include <vector>

#include "sl/ecs/Entity.h"

namespace sl::scene {

ecs::Entity& Scene::addEntity(std::string name) {
    return ecsRegistry.createEntity(std::move(name));
}

int Scene::getEntitiesCount() const { return ecsRegistry.getEntitiesCount(); }

void Scene::clear() {
    LOG_INFO("Cleaning up scene");

    skybox.reset();
    ecsRegistry.clear();
    vectors.clear();
}

}  // namespace sl::scene
