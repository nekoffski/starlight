#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "Scene.h"
#include "sl/ecs/Registry.h"
#include "sl/ecs/fwd.h"

#include "sl/scene/components/TransformComponentWrapper.h"

namespace sl::scene {

class Scene2D : public Scene {
    friend class SceneManager2D;

public:
    static std::shared_ptr<Scene2D> create() {
        auto scene = std::make_shared<Scene2D>();
        scene->setComponentWrapperFactory<components::TransformComponent,
            components::TransformComponentWrapperFactory>();
        return scene;
    }

private:
};
}
