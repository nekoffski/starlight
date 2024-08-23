#include "RenderTarget.hh"

namespace sl {

RenderTarget::RenderTarget(u32 id, const Properties& props) :
    m_id(id), m_props(props) {}

u32 RenderTarget::getId() const { return m_id; }

}  // namespace sl
