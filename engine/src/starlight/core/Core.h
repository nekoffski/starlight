#pragma once

#include <cstdint>
#include <string>

#include <kc/core/ErrorBase.hpp>
#include <tl/expected.hpp>

namespace sl {

DEFINE_ERROR(SLError);

using u64 = uint64_t;
using u32 = uint32_t;
using u16 = uint16_t;
using u8  = uint8_t;

struct Range {
    u64 offset;
    u64 size;

    static Range aligned(u64 offset, u64 size, u64 granularity);
};

template <typename T> using Expected = tl::expected<T, SLError>;

std::string_view removeExtension(std::string_view path);

}  // namespace sl
