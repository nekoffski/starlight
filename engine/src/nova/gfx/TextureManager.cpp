#include "TextureManager.h"

#include <kc/core/Log.h>
#include <fmt/core.h>

#include "Texture.h"
#include "TextureLoader.h"

namespace nova::gfx {

TextureManager::TextureManager(TextureLoader* textureLoader, std::string_view texturesPath)
    : m_textureLoader(textureLoader), m_texturesPath(texturesPath) {}

Texture* TextureManager::load(const std::string& name) {
    LOG_TRACE("Loading texture '{}'", name);
    static const std::string_view extension = "png";

    ASSERT(not m_textures.contains(name), "Texture {} already stored", name);

    const auto fullPath = fmt::format("{}/{}.{}", m_texturesPath, name, extension);
    m_textures[name]    = m_textureLoader->load(name, fullPath);
    return m_textures[name].get();
}

Texture* TextureManager::acquire(const std::string& name) {
    LOG_TRACE("Acquiring texture '{}'", name);
    if (auto texture = m_textures.find(name); texture != m_textures.end()) {
        return texture->second.get();
    } else {
        LOG_WARN("Texture {} not found", name);
        return nullptr;
    }
}

void TextureManager::destroy(const std::string& name) {
    LOG_TRACE("Destroying texture '{}'", name);
    if (auto texture = m_textures.find(name); texture != m_textures.end()) [[likely]]
        m_textures.erase(texture);
    else
        LOG_WARN("Attempt to destroy not existing texture - {}, will ignore", name);
}

void TextureManager::destroyAll() { m_textures.clear(); }

}  // namespace nova::gfx