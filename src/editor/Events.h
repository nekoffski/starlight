#pragma once

#include "EngineState.h"
#include "sl/event/Event.h"

namespace editor {

DECLARE_EVENT(EngineStateChanged, sl::event::EditorCategory) {
    explicit EngineStateChanged(EngineState state)
        : state(state) {
    }

    std::string asString() const override {
        return "[EngineStateChanged]";
    }

    EngineState state;
};

DECLARE_EVENT(EnterGameMode, sl::event::EditorCategory) {
    std::string asString() const override {
        return "[EnterGameMode]";
}
}
;
}