#pragma once

#include <memory>

#include <xvent/Event.h>

#include "Categories.h"
#include "sl/graphics/Cubemap.h"

namespace sl::event {

#define DECLARE_EVENT(Event, Category) struct Event : xvent::EventBase<Event, Category>

DECLARE_EVENT(AddEntityEvent, EditorCategory) {
    AddEntityEvent(std::string name)
        : name(std::move(name)) {}

    std::string name;
};

DECLARE_EVENT(SetSkyboxEvent, EditorCategory) {
    SetSkyboxEvent(std::shared_ptr<sl::graphics::Cubemap> cubemap)
        : cubemap(cubemap) {
    }

    std::shared_ptr<sl::graphics::Cubemap> cubemap;
};

DECLARE_EVENT(WindowResizedEvent, CoreCategory) {
    WindowResizedEvent(int width, int height)
        : width(width)
        , height(height) {
    }

    int width;
    int height;
};

DECLARE_EVENT(QuitEvent, CoreCategory){};
}
