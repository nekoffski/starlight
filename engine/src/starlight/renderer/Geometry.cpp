#include "Geometry.h"

namespace sl {

const Geometry::DataDescription& Geometry::getDataDescription() const {
    return m_dataDescription;
}

const Geometry::Properties& Geometry::getProperties() const { return m_props; }

u32 Geometry::getId() const { return m_id; }

Geometry::Geometry(const Properties& props, u32 id) : m_props(props), m_id(id) {}

}  // namespace sl