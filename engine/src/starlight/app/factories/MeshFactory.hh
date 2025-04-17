#pragma once

#include "starlight/core/Factory.hh"
#include "starlight/renderer/Mesh.hh"

namespace sl {

class MeshFactory : public Factory<MeshFactory, Mesh> {
public:
    explicit MeshFactory(Buffer& vertexBuffer, Buffer& indexBuffer);

    template <typename T>
    requires std::is_constructible_v<Mesh::Properties3D, const T&>
    kstd::SharedPtr<Mesh> create(const std::string& name, const T& properties) {
        return create(name, Mesh::Properties3D{ properties });
    }

    kstd::SharedPtr<Mesh> create(
      const std::string& name, const Mesh::Properties2D& config
    );
    kstd::SharedPtr<Mesh> create(
      const std::string& name, const Mesh::Properties3D& config
    );

    kstd::SharedPtr<Mesh> getCube();
    kstd::SharedPtr<Mesh> getUnitSphere();
    kstd::SharedPtr<Mesh> getPlane();

private:
    kstd::SharedPtr<Mesh> createMesh(
      const Mesh::Data& meshData, const std::string& name
    );

    void createDefaults();

    Buffer& m_vertexBuffer;
    Buffer& m_indexBuffer;

    kstd::SharedPtr<Mesh> m_unitSphere;
    kstd::SharedPtr<Mesh> m_plane;
    kstd::SharedPtr<Mesh> m_cube;
};

}  // namespace sl
