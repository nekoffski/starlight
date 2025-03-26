#pragma once

#include <vector>

#include "starlight/core/Json.hh"
#include "starlight/core/memory/Memory.hh"
#include "starlight/core/Concepts.hh"
#include "starlight/core/Id.hh"
#include "starlight/renderer/Mesh.hh"
#include "starlight/renderer/Material.hh"
#include "starlight/physx/Transform.hh"
#include "starlight/physx/bvh/BoundingVolume.hh"

namespace sl {

class Model : public NamedResource<Model, "Model", false> {
public:
    enum class Type { custom = 0, wavefront = 1 };

    struct Sub {
        SharedPtr<Mesh> mesh;
        SharedPtr<Material> material;
    };

    explicit Model(OptStr name = {});

    template <typename C>
    requires Callable<C, void, Sub&>
    void traverse(C&& callback) {
        for (auto& sub : m_subs) callback(sub);
    }

    Transform& getTransform();

    virtual Type getType() const                            = 0;
    virtual nlohmann::json serialize() const                = 0;
    virtual const BoundingVolume& getBoundingVolume() const = 0;

protected:
    Transform m_transform;
    std::vector<Sub> m_subs;
};

}  // namespace sl
