#include "String.hh"

namespace starlight {

std::vector<std::string> split(
    const std::string& input, const std::string& separator
) {
    std::vector<std::string> result;
    u64 start = 0;
    u64 end;

    while ((end = input.find(separator, start)) != std::string::npos) {
        result.push_back(input.substr(start, end - start));
        start = end + separator.length();
    }
    result.push_back(input.substr(start));
    return result;
}

std::string nameFromPath(const std::string& path, NameExtractionMode mode) {
    const auto begin = path.find_last_of('/') + 1;
    const auto end = mode == NameExtractionMode::withExtension
                         ? path.npos
                         : (mode == NameExtractionMode::withoutFullExtension
                                ? path.find_first_of('.')
                                : path.find_last_of('.'));
    return path.substr(begin, end - begin);
}

std::optional<std::string> extensionFromPath(
    const std::string& path, ExtensionExtractionMode mode
) {
    const auto name = nameFromPath(path, NameExtractionMode::withExtension);
    const auto begin = mode == ExtensionExtractionMode::full
                           ? name.find_first_of('.')
                           : name.find_last_of('.');

    if (begin != path.npos) {
        return name.substr(begin);
    }
    return {};
}

std::string toBinaryString(CBytesView bytes) {
    std::string result;
    result.reserve(bytes.size() * 8);
    for (const auto byte : bytes) {
        result += std::bitset<8>(byte).to_string();
    }
    return result;
}

std::string toHexString(CBytesView bytes) {
    std::string result;
    result.reserve(bytes.size() * 2);
    for (const auto byte : bytes) {
        result += fmt::format("{:02x}", byte);
    }
    return result;
}

std::string hexDump(CBytesView bytes) {
    std::string hex;
    std::string ascii;
    hex.reserve(bytes.size() * 3);
    ascii.reserve(bytes.size());
    for (const auto byte : bytes) {
        hex += fmt::format("{:02x} ", byte);
        ascii += (byte >= 0x20 && byte < 0x7f) ? static_cast<char>(byte) : '.';
    }
    if (!hex.empty()) {
        hex.pop_back();
    }
    return fmt::format("{} | {}", hex, ascii);
}

}  // namespace starlight