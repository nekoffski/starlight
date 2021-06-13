#pragma once

#include <string>

namespace editor {

enum class EngineMode {
    inGame,
    inEditor
};

inline std::string toString(EngineMode engineState) {
    switch (engineState) {
    case EngineMode::inGame:
        return "in_game";

    case EngineMode::inEditor:
        return "in_editor";
    }

    return "";
}

}