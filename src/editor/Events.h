#pragma once

#include "EngineState.h"
#include "sl/event/Event.h"

namespace editor {

DECLARE_EVENT(EngineStateChanged, sl::event::EditorCategory) {
    explicit EngineStateChanged(EngineState state)
        : state(state) {
    }

    EngineState state;
};

}