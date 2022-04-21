#pragma once

#include <memory>

#include "sl/ecs/Component.h"
#include "sl/ecs/Entity.h"
#include "sl/gfx/Shader.h"
#include "sl/physx/AxisAlignedBoundingBox.h"
#include "sl/physx/BoundingBox.h"

namespace sl::scene::components {

struct ModelComponent : ecs::Component {
    using View = ecs::ComponentView<ModelComponent>;

    explicit ModelComponent() {
        instances.push_back(math::Vec3{0.0f, 0.0f, 0.0f});

        boundingBox = std::make_unique<physx::AxisAlignedBoundingBox>(meshes);

        name = "ModelComponent";
    }

    void rebuildBoundingBox() {
        if (boundingBox) [[likely]]
            boundingBox->rebuild(meshes);
    }

    std::vector<std::shared_ptr<geom::Mesh>> meshes;
    std::vector<math::Vec3> instances;
    std::unique_ptr<physx::BoundingBox> boundingBox;
};
}  // namespace sl::scene::components
