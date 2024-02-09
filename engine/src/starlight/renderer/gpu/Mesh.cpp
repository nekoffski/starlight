#include "Mesh.h"

#include "starlight/core/math/Vertex.h"

namespace sl {

Mesh::Mesh(const Properties& props, const Data& data, u32 id) :
    m_props(props), m_id(id), m_extent(data.extent) {}

const Mesh::DataDescription& Mesh::getDataDescription() const {
    return m_dataDescription;
}

const Mesh::Properties& Mesh::getProperties() const { return m_props; }

const Extent3& Mesh::getExtent() const { return m_extent; }

u32 Mesh::getId() const { return m_id; }

}  // namespace sl