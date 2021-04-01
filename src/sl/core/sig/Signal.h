#pragma once

#include <csignal>

#include "SignalHandler.h"
#include "sl/core/NotNullPtr.hpp"

namespace sl::core::sig {

extern void setupSignalHandler(core::NotNullPtr<SignalHandler>);
}