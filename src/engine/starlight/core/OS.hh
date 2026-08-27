#pragma once

#include "Core.hh"

#ifdef _WIN32
#define STARLIGHT_PLATFORM_WINDOWS
#elif __APPLE__
#define STARLIGHT_PLATFORM_DARWIN
#else
#define STARLIGHT_PLATFORM_LINUX
#endif

namespace starlight {

enum class OS {
    linux,
    windows,
    darwin,
};

inline constexpr OS detectOs() {
#ifdef STARLIGHT_PLATFORM_WINDOWS
    return OS::windows;
#elif defined(STARLIGHT_PLATFORM_DARWIN)
    return OS::darwin;
#else
    return OS::linux;
#endif
}

Str toString(OS os);

}  // namespace starlight
