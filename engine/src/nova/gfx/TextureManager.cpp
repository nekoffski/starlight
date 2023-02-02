#include "TextureManager.h"

#include <kc/core/Log.h>
#include <fmt/core.h>

#include "Texture.h"
#include "TextureLoader.h"

namespace nova::gfx {

TextureManager::TextureManager(TextureLoader* textureLoader, std::string_view texturesPath)
    : m_textureLoader(textureLoader), m_texturesPath(texturesPath) {}

Texture* TextureManager::load(const std::string& name, const std::string& path) {
    ASSERT(not m_textures.contains(name), "Texture {} already stored", name);

    const auto fullPath = fmt::format("{}/{}", m_texturesPath, path);
    m_textures[name]    = m_textureLoader->load(name, fullPath);
    return m_textures[name].get();
}

Texture* TextureManager::acquire(const std::string& name) {
    ASSERT(m_textures.contains(name), "Texture {} not found", name);
    return m_textures[name].get();
}

void TextureManager::destroy(const std::string& name) {
    if (not m_textures.contains(name)) {
        LOG_WARN("Attempt to destroy not existing texture - {}, will ignore", name);
        return;
    }
    m_textures.erase(name);
}

void TextureManager::destroyAll() { m_textures.clear(); }

}  // namespace nova::gfx