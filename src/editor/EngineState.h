#pragma once

namespace editor {

enum class EngineState {
    paused,
    stopped,
    started
};

inline std::string toString(EngineState engineState) {
    switch (engineState) {
    case EngineState::paused:
        return "paused";

    case EngineState::stopped:
        return "stopped";

    case EngineState::started:
        return "started";
    }

    return "";
}

}