#pragma once

#include <vector>

#include "starlight/core/math/Transform.h"
#include "gpu/Mesh.h"

namespace sl {

class Model {
public:
    explicit Model(u64 id, std::span<Mesh*> meshes);

    Mat4f getModelMatrix();

    u64 getId() const;
    std::span<Mesh*> getMeshes();

    // TODO: get rid of
    void setTransform(Transform* transform) { m_transform = transform; }

private:
    u64 m_id;
    std::vector<Mesh*> m_meshes;
    Transform* m_transform;
};

}  // namespace sl
