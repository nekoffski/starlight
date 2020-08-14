#pragma once

#include <csignal>

#include "SignalHandler.h"
#include "starlight/misc/types/NotNullPtr.hpp"

namespace sl::core::sig {

extern void setupSignalHandler(misc::types::NotNullPtr<SignalHandler>);
}