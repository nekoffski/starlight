#pragma once

#include <memory>

namespace sl::core {

class Input;

class InputHandler {
public:
    virtual void handleInput(std::shared_ptr<Input>) = 0;
};
}
