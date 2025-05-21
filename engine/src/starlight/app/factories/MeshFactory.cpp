#include "MeshFactory.hh"

namespace sl {

MeshFactory::MeshFactory(Buffer& vertexBuffer, Buffer& indexBuffer)
    : m_vertexBuffer(vertexBuffer)
    , m_indexBuffer(indexBuffer) {
    createDefaults();
}

kstd::SharedPtr<Mesh> MeshFactory::create(
  const std::string& name, const Mesh::Properties2D& config
) {
    return save(createMesh(config.toMeshData(), name));
}

kstd::SharedPtr<Mesh> MeshFactory::create(
  const std::string& name, const Mesh::Properties3D& config
) {
    return save(createMesh(config.toMeshData(), name));
}

kstd::SharedPtr<Mesh> MeshFactory::getCube() { return m_cube; }
kstd::SharedPtr<Mesh> MeshFactory::getUnitSphere() { return m_unitSphere; }
kstd::SharedPtr<Mesh> MeshFactory::getPlane() { return m_plane; }

void MeshFactory::createDefaults() {
    Mesh::Properties3D unitSphere{
        SphereProperties{ 16, 16, 1.0f }
    };
    m_unitSphere = save(createMesh(unitSphere.toMeshData(), "UnitSphere"));

    Mesh::Properties3D plane{
        PlaneProperties{ 5.0f, 5.0f, 2, 2 }
    };
    m_plane = save(createMesh(plane.toMeshData(), "Plane"));

    Mesh::Properties3D cube{
        CubeProperties{ 1.0f, 1.0f, 1.0f, 1, 1 }
    };
    m_cube = save(createMesh(cube.toMeshData(), "Cube"));
}

kstd::SharedPtr<Mesh> MeshFactory::createMesh(
  const Mesh::Data& meshData, const std::string& name
) {
    log::info("Creating mesh: {}", name);
    return kstd::makeShared<Mesh>(meshData, m_vertexBuffer, m_indexBuffer, name);
}

}  // namespace sl
