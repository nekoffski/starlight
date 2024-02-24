#include "Texture.h"

namespace sl {

const Texture::Properties& Texture::getProperties() const { return m_props; }

const u32 Texture::getId() const { return m_id; }

Texture::Texture(const Properties& props, u32 id) : m_props(props), m_id(id) {}

}  // namespace sl
