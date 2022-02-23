#pragma once

namespace sl::core {

struct Keyboard {
    virtual bool isKeyPressed(int) const = 0;
};

}  // namespace sl::core