#include "Geometry.h"

#include "starlight/core/math/Vertex.h"

namespace sl {

Geometry::Geometry(const Properties& props, const Data& data, u32 id) :
    m_props(props), m_id(id), m_extent(data.extent) {}

const Geometry::DataDescription& Geometry::getDataDescription() const {
    return m_dataDescription;
}

const Geometry::Properties& Geometry::getProperties() const { return m_props; }

const Extent3& Geometry::getExtent() const { return m_extent; }

u32 Geometry::getId() const { return m_id; }

}  // namespace sl