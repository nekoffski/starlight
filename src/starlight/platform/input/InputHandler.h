#pragma once

#include <memory>

namespace starl::platform::input {

class Input;

class InputHandler {
public:
    virtual void handleInput(std::unique_ptr<Input>&) = 0;
};
}