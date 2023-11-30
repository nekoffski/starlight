#pragma once

#include "starlight/renderer/RenderTarget.h"
#include "starlight/core/memory/Memory.hpp"

#include "VKDevice.h"
#include "VKContext.h"
#include "VKFramebuffer.h"

namespace sl::vk {

class VKRenderTarget : public RenderTarget {
public:
    explicit VKRenderTarget(
      u32 id, VKContext& context, VKDevice& device, const Properties& props
    );

    void regenerate(const Properties& properties) override;

private:
    VKContext& m_context;
    VKDevice& m_device;

    LocalPtr<VKFramebuffer> m_framebuffer;
};

}  // namespace sl::vk
