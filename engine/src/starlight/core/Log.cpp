#include "Log.h"

#include <atomic>

std::atomic_bool sl_isVariableLoggingEnabled;

namespace sl {

void enableVariableLogging() { sl_isVariableLoggingEnabled = true; }

void disableVariableLogging() { sl_isVariableLoggingEnabled = false; }

bool isVariableLoggingEnabled() { return sl_isVariableLoggingEnabled; }

}  // namespace sl