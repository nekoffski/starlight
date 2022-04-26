#pragma once

#include <kc/core/Singleton.hpp>

#include "CollisionProcessor.h"
#include "sl/scene/components/RigidBodyComponent.h"
#include "sl/scene/components/TransformComponent.h"

namespace sl::physx {

class PhysicsEngine : public kc::core::Singleton<PhysicsEngine> {
   public:
    using BoundingBoxes = std::unordered_map<std::string, BoundingBox*>;

    void processRigidBodies(
        scene::components::RigidBodyComponent::View rigidBodies,
        scene::components::TransformComponent::View transforms, const BoundingBoxes& boundingBoxes,
        float deltaTime
    );

   private:
    CollisionProcessor m_collisionProcessor;
};
}  // namespace sl::physx