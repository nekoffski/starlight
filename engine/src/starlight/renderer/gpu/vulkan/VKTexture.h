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

private:
    const VKContext* m_context;
    VKDevice* m_device;

    LocalPtr<VKImage> m_image;
    u32 m_generation;
};

class VKTextureMap : public TextureMap {
public:
    explicit VKTextureMap(
      VKContext& context, VKDevice& device, const Properties& props, u32 id,
      VKTexture& texture
    );
    ~VKTextureMap() override;
    Texture* getTexture() const override;

    const VKImage* getImage() const;
    VkSampler getSampler() const;

private:
    VKContext& m_context;
    VKDevice& m_device;
    VKTexture& m_texture;

    VkSampler m_sampler;
};

}  // namespace sl::vk
