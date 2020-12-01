#pragma once

#include "sl/platform/texture/Cubemap.h"
#include <memory>

namespace sl::core::event {

enum class EventCategory : unsigned char {
    EDITOR
};

enum class EventType : unsigned char {
    ADD_ENTITY,
    SET_SKYBOX
};

struct Event : public std::enable_shared_from_this<Event> {
    virtual EventCategory getCategory() = 0;
    virtual EventType getType() = 0;

    bool isHandled = false;

    template <typename T>
    std::shared_ptr<T> as() {
        return std::dynamic_pointer_cast<T>(shared_from_this());
    }
};

// clang-format off
#define SL_EVENT(category, type)\
	EventCategory getCategory() { return category; }\
	EventType getType() { return type; }
// clang-format on

struct AddEntityEvent : Event {
    SL_EVENT(EventCategory::EDITOR, EventType::ADD_ENTITY);

    AddEntityEvent(std::string name)
        : name(std::move(name)) {}

    std::string name;
};

struct SetSkyboxEvent : Event {
    SL_EVENT(EventCategory::EDITOR, EventType::SET_SKYBOX);

    SetSkyboxEvent(std::shared_ptr<sl::platform::texture::Cubemap> cubemap)
        : cubemap(cubemap) {
    }

    std::shared_ptr<sl::platform::texture::Cubemap> cubemap;
};
}
