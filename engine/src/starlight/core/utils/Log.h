#pragma once

#include <kc/core/Log.h>

namespace sl {

#define LOG_VARS 1

using kc::core::initLogging;

#define FAIL(...) ASSERT(false, __VA_ARGS__)

#ifdef LOG_VARS
#define LOG_VAR(var) LOG_WARN(#var "={}", var)
#else
#define LOG_VAR(var)
#endif

}  // namespace sl
