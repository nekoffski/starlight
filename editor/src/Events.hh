#pragma once

namespace sle::events {

struct SceneSerialization {
    enum class Action { serialize, deserialize };

    Action action;
    std::string path;
};

}  // namespace sle::events
