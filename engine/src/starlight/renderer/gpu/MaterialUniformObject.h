#pragma once

#include "starlight/core/math/Glm.h"

namespace sl {

struct MaterialUniformObject {
    Vec4f diffuseColor;  // 16 bytes
    Vec4f reserved0;     // 16 bytes, reserved for future use
    Vec4f reserved1;     // 16 bytes, reserved for future use
    Vec4f reserved2;     // 16 bytes, reserved for future use
};

}  // namespace sl