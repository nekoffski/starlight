#pragma once

#include "Keyboard.h"
#include "Mouse.h"

namespace sl::core {

struct Input : Mouse, Keyboard {
    virtual ~Input() = default;

    virtual void init(void*) = 0;
};

}