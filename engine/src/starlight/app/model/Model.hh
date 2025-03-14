#pragma once

#include <vector>

#include "starlight/core/Json.hh"
#include "starlight/core/memory/Memory.hh"
#include "starlight/core/Concepts.hh"
#include "starlight/core/math/Transform.hh"
#include "starlight/core/Id.hh"
#include "starlight/renderer/Mesh.hh"
#include "starlight/renderer/Material.hh"

namespace sl {

class Model : public NamedResource<Model, "Model", false> {
public:
    enum class Type { custom = 0, wavefront = 1 };

    struct Sub {
        SharedPtr<Mesh> mesh;
        SharedPtr<Material> material;
    };

    explicit Model(OptStr name = {});

    Transform& getTransform();

    template <typename C>
    requires Callable<C, void, Mesh&, Material&>
    void traverse(C&& callback) {
        for (auto& [mesh, material] : m_subs) callback(*mesh, *material);
    }

    virtual Type getType() const             = 0;
    virtual nlohmann::json serialize() const = 0;

protected:
    std::vector<Sub> m_subs;
    Transform m_transform;
};

}  // namespace sl
