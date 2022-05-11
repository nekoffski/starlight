#pragma once

#include <string_view>

#include "nova/math/Size.hpp"

namespace nova::core {

struct Window {
    virtual std::string_view getVendor() const = 0;
    virtual void update()                      = 0;
    virtual void swapBuffers()                 = 0;
    virtual math::Size2i getSize() const       = 0;
};

}  // namespace nova::core