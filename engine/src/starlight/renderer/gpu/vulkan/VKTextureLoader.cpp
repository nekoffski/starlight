#include "VKTextureLoader.h"

#include "VKDevice.h"
#include "VKContext.h"
#include "VKTexture.h"

// #include "starlight/resource/ImageData.h"

namespace sl::vk {

VKTextureLoader::VKTextureLoader(const VKContext* context, VKDevice* device) :
    m_context(context), m_device(device) {}

UniqPtr<Texture> VKTextureLoader::load(
  const std::string& name, const Texture::Properties& props, const void* pixels
) const {
    return createUniqPtr<VKTexture>(m_context, m_device, name, props, pixels);
}

}  // namespace sl::vk
