#pragma once

#include <string>
#include <vector>

#include <stb.h>

#include <kc/core/Log.h>

#include "starlight/core/memory/Memory.hpp"
#include "starlight/renderer/Texture.h"

#include "Vulkan.h"
#include "VKImage.h"
#include "VKBuffer.h"
#include "VKContext.h"
#include "VKDevice.h"
#include "VKCommandBuffer.h"

namespace sl::vk {

class VKTexture : public Texture {
public:
    explicit VKTexture(
      const VKContext* context, VKDevice* device, const Properties& props, u32 id,
      const void* pixels
    );

    ~VKTexture() override;

    const VKImage* getImage() const;
    VkSampler getSampler() const;

private:
    const VKContext* m_context;
    VKDevice* m_device;

    LocalPtr<VKImage> m_image;
    VkSampler m_sampler;

    u32 m_generation;
};

}  // namespace sl::vk
