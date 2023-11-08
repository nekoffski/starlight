#pragma once

#include <string>
#include <vector>

#include <stb.h>

#include <kc/core/Log.h>

#include "starlight/core/memory/Memory.hpp"
#include "starlight/core/utils/Id.h"
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

    VKImage* getImage();
    VkSampler getSampler();

private:
    const VKContext* m_context;
    VKDevice* m_device;

    LocalPtr<VKImage> m_image;
    VkSampler m_sampler;
};

}  // namespace sl::vk
