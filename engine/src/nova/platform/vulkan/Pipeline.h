#pragma once

#include "Vulkan.h"

namespace nova::platform::vulkan {

class Pipeline {
   public:
   private:
    VkPipeline m_handle;
    VkPipelineLayout m_layout;
};

}  // namespace nova::platform::vulkan
