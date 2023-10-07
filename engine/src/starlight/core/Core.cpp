#include "Core.h"

namespace sl {

std::string_view removeExtension(std::string_view path) {
    if (const auto extension = path.find("."); extension != path.npos)
        return std::string_view(path.begin(), extension);
    return path;
}

template <typename T> T getAlignedValue(T value, T granularity) {
    return (value + granularity - 1) & ~(granularity - 1);
}

Range Range::aligned(u64 offset, u64 size, u64 granularity) {
    return Range(
      getAlignedValue(offset, granularity), getAlignedValue(size, granularity)
    );
}

}  // namespace sl
