#pragma once

#include <concepts>

namespace sl::platform::clock::tickers {

struct Ticker {
    virtual float getTicks() = 0;
};
}

template <typename T>
concept TickerPolicy = std::derived_from<T, sl::platform::clock::tickers::Ticker>;