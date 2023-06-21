#include "TextureLoader.h"

#include "Device.h"
#include "Context.h"
#include "Texture.h"

// #include "starlight/resource/ImageData.h"

namespace sl::vk {

TextureLoader::TextureLoader(const Context* context, Device* device)
    : m_context(context), m_device(device) {}

UniqPtr<sl::Texture> TextureLoader::load(
    const std::string& name, const Texture::Properties& props, const void* pixels
) const {
    return createUniqPtr<Texture>(m_context, m_device, name, props, pixels);
}

}  // namespace sl::vk
