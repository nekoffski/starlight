#pragma once

#include "VKContext.h"
#include "VKLogicalDevice.h"
#include "VKPhysicalDevice.h"

namespace sl::vk {

struct VKBackendAccessor {
    virtual VKContext* getContext()               = 0;
    virtual VKPhysicalDevice* getPhysicalDevice() = 0;
    virtual VKLogicalDevice* getLogicalDevice()   = 0;
};

}  // namespace sl::vk
