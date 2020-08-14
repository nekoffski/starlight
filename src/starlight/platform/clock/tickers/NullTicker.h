#pragma once

#include "Ticker.h"
#include "starlight/core/log/Logger.h"

namespace sl::platform::clock::tickers {

struct NullTicker : public Ticker {
    float getTicks() override {
        SL_WARN("Clock uses NullTicker, delta time is always 0.0f!");
        return 0.0f;
    }
};
}