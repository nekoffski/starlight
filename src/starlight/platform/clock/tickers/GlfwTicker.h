#pragma once

#include "Ticker.h"

namespace sl::platform::clock::tickers {

struct GlfwTicker : public Ticker {
    float getTicks() override;
};
}