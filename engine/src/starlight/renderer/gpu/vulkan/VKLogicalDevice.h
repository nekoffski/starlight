#pragma once

#include "VKPhysicalDevice.h"

namespace sl::vk {

class VKLogicalDevice {
public:
    explicit VKLogicalDevice(VKPhysicalDevice& physicalDevice) :
        m_physicalDevice(physicalDevice) {}

private:
    VKPhysicalDevice& m_physicalDevice;
};

}  // namespace sl::vk
