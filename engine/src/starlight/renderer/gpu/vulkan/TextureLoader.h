#pragma once

#include "fwd.h"

#include "starlight/renderer/Texture.h"
#include "starlight/renderer/gpu/TextureLoader.h"

namespace sl::vk {

class TextureLoader : public sl::TextureLoader {
   public:
    explicit TextureLoader(const Context* context, Device* device);

    UniqPtr<sl::Texture> load(
        const std::string& name, const sl::Texture::Properties& props, const void* pixels
    ) const override;

   private:
    const Context* m_context;
    Device* m_device;
};

}  // namespace sl::vk
