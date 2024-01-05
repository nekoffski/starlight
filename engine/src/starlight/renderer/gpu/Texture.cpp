#include "Texture.h"

namespace sl {

const Texture::Properties& Texture::getProperties() const { return m_props; }

const u32 Texture::getId() const { return m_id; }

Texture::Texture(const Properties& props, u32 id) : m_props(props), m_id(id) {}

const TextureMap::Properties& TextureMap::getProperties() const { return m_props; }

u64 TextureMap::getId() const { return m_id; }

TextureMap::TextureMap(const Properties& props, u64 id) : m_props(props), m_id(id) {}

}  // namespace sl