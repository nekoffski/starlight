#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "Scene.h"
#include "Skybox.h"
#include "sl/ecs/Registry.h"
#include "sl/ecs/fwd.h"

namespace sl::scene {

class Scene3D : public Scene {
    friend class SceneManager3D;

public:
    static std::shared_ptr<Scene3D> create();

    void setSkybox(std::shared_ptr<Skybox> skybox);


private:
    std::shared_ptr<Skybox> m_skybox;
};
}
