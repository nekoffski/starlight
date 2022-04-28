#pragma once

#include <kc/core/Singleton.hpp>

#include "CollisionProcessor.h"
#include "sl/scene/Scene.h"

namespace sl::physx {

class PhysicsEngine : public kc::core::Singleton<PhysicsEngine> {
   public:
    void processRigidBodies(scene::Scene& scene, float deltaTime);

   private:
    CollisionProcessor m_collisionProcessor;
};
}  // namespace sl::physx