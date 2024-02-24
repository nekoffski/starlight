#pragma once

#include <string>
#include <vector>

#include <stb.h>

#include <kc/core/Log.h>

#include "starlight/core/memory/Memory.hpp"
#include "starlight/renderer/gpu/Texture.h"

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
      u32 id, const VKContext* context, VKDevice* device, const Properties& props,
      const std::span<u8> pixels
    );

    // TODO: consider splitting those classes somehow
    explicit VKTexture(
      u32 id, const VKContext* context, VKDevice* device, const Properties& props,
      VkImage handle, VkFormat format
    );

    explicit VKTexture(
      u32 id, const VKContext* context, VKDevice* device,
      const VKImage::Properties& props
    );

    ~VKTexture() override;

    const VKImage* getImage() const;
    VkSampler getSampler() const;

    void resize(u32 width, u32 height, VkImage handle);
    void resize(u32 width, u32 height) override;

    void write(u32 offset, std::span<u8> pixels) override;

private:
    void createSampler(const Texture::Properties& props);

    const VKContext* m_context;
    VKDevice* m_device;

    VKImage m_image;
    VkSampler m_sampler;

    u32 m_generation;
};

// class VKTextureMap : public TextureMap {
// public:
//     explicit VKTextureMap(
//       u32 id, VKContext& context, VKDevice& device, const Properties& props,
//       VKTexture& texture
//     );
//     ~VKTextureMap() override;
//     Texture* getTexture() const override;

//     const VKImage* getImage() const;
//     VkSampler getSampler() const;

// private:
//     VKContext& m_context;
//     VKDevice& m_device;
//     VKTexture& m_texture;

//     VkSampler m_sampler;
// };

}  // namespace sl::vk
