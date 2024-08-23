#pragma once

#include <cstdint>

namespace sl::vk {

struct VKDescriptorState {
    uint32_t generations[3];  // generation per frame
    uint32_t ids[3];          // generation per frame
};

}  // namespace sl::vk