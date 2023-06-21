#pragma once

#include "starlight/core/math/Glm.h"

namespace sl {

struct GlobalUniformObject {
    Mat4f projection;
    Mat4f view;
    // pad to 256
    Mat4f p0, p1;
};

}  // namespace sl
