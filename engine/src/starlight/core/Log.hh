#pragma once

#include <kc/core/Log.h>

namespace sl {

#define LOG_VARS 1

using kc::core::initLogging;

#define FATAL_ERROR(...) ASSERT(false, __VA_ARGS__)

void enableVariableLogging();
void disableVariableLogging();
bool isVariableLoggingEnabled();

#ifdef LOG_VARS
#define LOG_VAR(var)                                                              \
    do {                                                                          \
        if (sl::isVariableLoggingEnabled()) LOG_WARN("VAR '" #var "' = {}", var); \
    } while (false)
#else
#define LOG_VAR(var)
#endif

}  // namespace sl
