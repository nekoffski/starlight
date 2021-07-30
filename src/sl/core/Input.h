#pragma once

#include "Keyboard.h"
#include "Mouse.h"

namespace sl::core {

struct Input : Mouse, Keyboard {
    virtual void init(void*) = 0;
};

}