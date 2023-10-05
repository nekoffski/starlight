#pragma once

#include <kc/core/Log.h>

namespace sl {

using kc::core::initLogging;

#define FAIL(...) ASSERT(false, __VA_ARGS__)

}  // namespace sl
