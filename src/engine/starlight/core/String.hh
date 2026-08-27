#pragma once

#include <algorithm>
#include <bitset>
#include <concepts>
#include <optional>
#include <span>
#include <string>
#include <vector>

#include "Core.hh"

namespace starlight {

template <u64 N>
struct StringLiteral {
    constexpr StringLiteral(const char (&str)[N]) {
        std::copy_n(str, N, value);
    }
    char value[N];
};

enum class NameExtractionMode {
    withExtension,
    withoutFullExtension,
    withoutLastExtensionChunk
};
enum class ExtensionExtractionMode { full, lastChunk };

std::string nameFromPath(
    const std::string& path,
    NameExtractionMode mode = NameExtractionMode::withoutFullExtension
);

std::optional<std::string> extensionFromPath(
    const std::string& path,
    ExtensionExtractionMode mode = ExtensionExtractionMode::full
);

std::vector<std::string> split(
    const std::string& input, const std::string& separator
);

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

}  // namespace starlight
