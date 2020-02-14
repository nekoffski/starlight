#pragma once

#include <string_view>

namespace {
    constexpr int unix__ = 1;
    constexpr int win32__ = 2;
    constexpr int win64__ = 3;
    constexpr int mac__ = 4;
}

#ifdef __unix__
    #define STARL_PLATFORM_UNIX
    namespace {
        constexpr int pdPlatformId = unix__;
    }
#elif _WIN32
    #ifdef _WIN64
        #define STARL_PLATFORM_WIN64
        namespace {
            constexpr int pdPlatformId = win32__;
        }   
    #else
        #define STARL_PLATFORM_WIN32
        namespace {
            constexpr int pdPlatformId = win64__;
        }
    #endif
#elif __APPLE__
    #define STARL_PLATFORM_MAC
    namespace {
        constexpr int pdPlatformId = mac__;
    }
#else 
    #define STARL_PLATFORM_UNKNOWN
#endif

constexpr std::string_view STARL_GET_PLATFORM_NAME() {
    switch (pdPlatformId) {
        case unix__:
            return "unix";
        case win32__:
            return "win32";
        case win64__:
            return "win64";
        case mac__:
            return "mac";
        default:
            return "unrecognized";
    }
}
