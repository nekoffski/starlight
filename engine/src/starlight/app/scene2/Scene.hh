#pragma once

#include "starlight/core/Id.hh"
#include "starlight/core/memory/Memory.hh"

#include "starlight/renderer/Skybox.hh"
#include "starlight/renderer/RenderPacket.hh"

#include "ecs/Entity.hh"
#include "ecs/EntityManager.hh"

namespace sl::v2 {

class Scene : public NamedResource<Scene, "Scene"> {
public:
    RenderPacket assembleRenderPacket();

    void setSkybox(SharedPtr<Skybox>);
    void resetSkybox();

    Entity& addEntity(OptStr name = {});
    bool hasEntity(const std::string& name);
    Entity* getEntity(const std::string& name);

private:
    EntityManager m_entityManager;
    SharedPtr<Skybox> m_skybox;
};

}  // namespace sl::v2
