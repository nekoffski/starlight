#pragma once

#include "Core.hh"

#ifdef _WIN32
#define SL_PLATFORM_WINDOWS
#elif __APPLE__
#define SL_PLATFORM_DARWIN
#else
#define SL_PLATFORM_LINUX
#endif

namespace sl {

enum class OS {
    linux,
    windows,
    darwin,
};

inline constexpr OS detectOs() {
#ifdef SL_PLATFORM_WINDOWS
    return OS::windows;
#elif defined(SL_PLATFORM_DARWIN)
    return OS::darwin;
#else
    return OS::linux;
#endif
}

Str toString(OS os);

}  // namespace sl
