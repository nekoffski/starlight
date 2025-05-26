#pragma once

#include <cstdint>
#include <cstring>

#include <limits>
#include <string>

#include <kstd/memory/SharedPtr.hh>
#include <kstd/memory/UniquePtr.hh>
#include <kstd/memory/LocalPtr.hh>
#include <kstd/Concepts.hh>
#include <kstd/FileSystem.hh>

namespace sl {

using u64 = uint64_t;
using u32 = uint32_t;
using u16 = uint16_t;
using u8  = uint8_t;

using Nanoseconds = u64;

using i64 = int64_t;
using i32 = int32_t;
using i16 = int16_t;
using i8  = int8_t;

using f32 = float;
using f64 = double;

template <typename T> T max() { return std::numeric_limits<T>::max(); }
template <typename T> T min() { return std::numeric_limits<T>::min(); }

}  // namespace sl
