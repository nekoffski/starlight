#pragma once

#include <string>

#include "sl/math/Matrix.hpp"
#include "sl/math/Vector.hpp"

#include <kc/math/Format.h>

namespace sl::physx {

struct Vector {
    math::Vec3 origin;
    math::Vec3 direction;
};

inline std::string toString(const Vector& vector) {
    return fmt::format("Vector[ origin={}, direction={} ]", vector.origin, vector.direction);
}

TO_STRING_STREAM(Vector);

}  // namespace sl::physx