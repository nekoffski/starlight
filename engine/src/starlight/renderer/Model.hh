#pragma once

#include <vector>
#include <unordered_map>

#include "starlight/core/math/Transform.hh"
#include "gpu/Mesh.hh"

namespace sl {

class Model {
public:
    explicit Model(u64 id);

    void addMesh(Material* material, Mesh* mesh);

    Mat4<f32> getModelMatrix();
    u64 getId() const;

    template <typename Callback>
    requires Callable<Callback, void, Material*, std::span<Mesh*>>
    void forEachMaterial(Callback&& callback) {
        for (auto& [id, material] : m_materials)
            callback(material, m_materialIdToMeshes[id]);
    }

    template <typename Callback>
    requires Callable<Callback, void, Mesh*>
    void forEachMesh(Callback&& callback) {
        for (auto& meshes : m_materialIdToMeshes | std::views::values)
            for (auto& mesh : meshes) callback(mesh);
    }

    // TODO: get rid of
    void setTransform(Transform* transform) { m_transform = transform; }

private:
    u64 m_id;

    // TODO: use vector
    std::unordered_map<u64, Material*> m_materials;
    std::unordered_map<u64, std::vector<Mesh*>> m_materialIdToMeshes;

    Transform* m_transform;
};

}  // namespace sl
