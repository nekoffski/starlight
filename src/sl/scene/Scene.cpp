#include "Scene.h"

#include <memory>
#include <unordered_map>
#include <vector>

#include "sl/ecs/Entity.h"

namespace sl::scene {

std::shared_ptr<Scene> Scene::create() {
    auto scene = std::make_shared<Scene>();
    return scene;
}
}
