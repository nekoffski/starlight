#include "TextureLoader.h"

#include "Device.h"
#include "Context.h"
#include "Texture.h"

namespace nova::platform::vulkan {

TextureLoader::TextureLoader(const Context* context, Device* device)
    : m_context(context), m_device(device) {}

core::UniqPtr<gfx::Texture> TextureLoader::load(
    const std::string& name, const Texture::Properties& props, const void* pixels
) const {
    return core::createUniqPtr<Texture>(
        m_context, m_device, name, props.width, props.height, props.channels, pixels,
        props.isTransparent
    );
}

core::UniqPtr<gfx::Texture> TextureLoader::load(const std::string& name, const std::string& path)
    const {
    return core::createUniqPtr<ImageTexture>(m_context, m_device, name, path);
}

}  // namespace nova::platform::vulkan
