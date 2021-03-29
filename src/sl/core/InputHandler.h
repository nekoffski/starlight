#pragma once

#include <memory>

#include "fwd.h"

namespace sl::core {

class InputHandler {
public:
    virtual void handleInput(Input&) = 0;
};
}
