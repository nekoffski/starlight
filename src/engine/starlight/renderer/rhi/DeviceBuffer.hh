#pragma once

#include "starlight/core/Core.hh"

namespace sl {

enum class DeviceBufferUsage { vertexBuffer, indexBuffer };

struct DeviceBufferDescription {
    DeviceBufferUsage usage;
    Bytes bytes;
};

}  // namespace sl
