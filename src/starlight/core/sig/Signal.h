#pragma once

#include <csignal>

#include <starlight/core/sig/SignalHandler.h>
#include <starlight/misc/types/NotNullPtr.hpp>

namespace starl::core::sig {

extern void setupSignalHandler(misc::types::NotNullPtr<SignalHandler>);
}