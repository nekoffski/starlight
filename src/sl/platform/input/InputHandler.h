#pragma once

#include <memory>

#include "fwd.h"

namespace sl::platform::input {
class InputHandler {
public:
    virtual void handleInput(std::shared_ptr<Input>) = 0;
};
}