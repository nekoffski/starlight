#pragma once

#include <memory>

#include "fwd.h"

namespace sl::core {

class InputHandler {
public:
    virtual void handleInput(std::shared_ptr<Input>) = 0;
};
}
