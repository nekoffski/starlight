#pragma once

#include "fwd.h"

#include "starlight/renderer/Texture.h"
#include "starlight/renderer/gpu/TextureLoader.h"

namespace sl::vk {

class VKTextureLoader : public TextureLoader {
public:
    explicit VKTextureLoader(const VKContext* context, VKDevice* device);

    UniqPtr<Texture> load(
      const std::string& name, const Texture::Properties& props, const void* pixels
    ) const override;

private:
    const VKContext* m_context;
    VKDevice* m_device;
};

}  // namespace sl::vk
