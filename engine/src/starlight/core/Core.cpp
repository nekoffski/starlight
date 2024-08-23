#include "Core.hh"

namespace sl {

std::string_view removeExtension(std::string_view path) {
    if (const auto extension = path.find("."); extension != path.npos)
        return std::string_view(path.begin(), extension);
    return path;
}

Range Range::aligned(u64 offset, u64 size, u64 granularity) {
    return Range(
      getAlignedValue(offset, granularity), getAlignedValue(size, granularity)
    );
}

}  // namespace sl
