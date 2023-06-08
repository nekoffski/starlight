#include "TextureLoader.h"

#include "Device.h"
#include "Context.h"
#include "Texture.h"

#include "starlight/res/ImageData.h"

namespace sl::platform::vulkan {

TextureLoader::TextureLoader(const Context* context, Device* device)
    : m_context(context), m_device(device) {}

core::UniqPtr<gfx::Texture> TextureLoader::load(
    const std::string& name, const Texture::Properties& props, const void* pixels
) const {
    return core::createUniqPtr<Texture>(m_context, m_device, name, props, pixels);
}

core::UniqPtr<gfx::Texture> TextureLoader::load(const std::string& name) const {
    const auto imageData = res::ImageData::create(name);

    Texture::Properties props{
        .width         = imageData->width,
        .height        = imageData->height,
        .channels      = imageData->channels,
        .isTransparent = imageData->isTransparent,
    };
    return load(name, props, imageData->pixels);
}

}  // namespace sl::platform::vulkan
