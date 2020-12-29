#pragma once

#include <csignal>

#include "SignalHandler.h"
#include "sl/core/types/NotNullPtr.hpp"

namespace sl::core::sig {

extern void setupSignalHandler(core::types::NotNullPtr<SignalHandler>);
}