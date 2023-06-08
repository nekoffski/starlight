#pragma once

#include "fwd.h"

#include "starlight/gfx/Texture.h"
#include "starlight/gfx/TextureLoader.h"

namespace sl::platform::vulkan {

class TextureLoader : public gfx::TextureLoader {
   public:
    explicit TextureLoader(const Context* context, Device* device);

    core::UniqPtr<gfx::Texture> load(
        const std::string& name, const gfx::Texture::Properties& props, const void* pixels
    ) const override;

    core::UniqPtr<gfx::Texture> load(const std::string& name) const override;

   private:
    const Context* m_context;
    Device* m_device;
};

}  // namespace sl::platform::vulkan
