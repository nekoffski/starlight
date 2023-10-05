#pragma once

#include <string>
#include <vector>

#include <stb.h>

#include <kc/core/Log.h>

#include "starlight/core/memory/Memory.hpp"
#include "starlight/core/utils/Id.h"
#include "starlight/renderer/Texture.h"

#include "Vulkan.h"
#include "Image.h"
#include "Buffer.h"
#include "Context.h"
#include "Device.h"
#include "CommandBuffer.h"

namespace sl::vk {

class Texture : public sl::Texture {
public:
    explicit Texture(
      const Context* context, Device* device, const std::string& name,
      const Properties& props, const void* pixels
    );

    ~Texture();

    Image* getImage();
    VkSampler getSampler();

protected:
    explicit Texture(const Context* context, Device* device);

    void create(
      const std::string& name, const Properties& props, const void* pixels
    );

private:
    const Context* m_context;
    Device* m_device;

    LocalPtr<Image> m_image;
    VkSampler m_sampler;
};

}  // namespace sl::vk
