#pragma once

#include "starlight/renderer/RenderPacket.h"

#include "ecs/Entity.h"

namespace sl {

class Scene {
public:
    Entity* addEntity(const std::string& name) {}
    RenderPacket getRenderPacket();

private:
};

}  // namespace sl
