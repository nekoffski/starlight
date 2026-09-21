#pragma once

#include <algorithm>
#include <bitset>
#include <concepts>
#include <optional>
#include <span>
#include <string>
#include <vector>

#include "Core.hh"

namespace sl {

template <u64 N>
struct StringLiteral {
    constexpr StringLiteral(const char (&str)[N]) {
        std::copy_n(str, N, value);
    }
    char value[N];
};

template <std::unsigned_integral T>
std::string toBinaryString(T value) {
    return std::bitset<sizeof(T) * 8>(value).to_string();
}

std::string toBinaryString(CBytesView bytes);

template <std::unsigned_integral T>
std::string toHexString(T value) {
    return fmt::format("{:0{}x}", value, sizeof(T) * 2);
}

std::string toHexString(CBytesView bytes);

std::string hexDump(CBytesView bytes);

}  // namespace sl
