#pragma once

#include <memory>

#include <xvent/Event.h>

#include "Categories.h"
#include "Util.h"

#include "sl/gfx/Cubemap.h"
#include "sl/math/Vector.hpp"

namespace sl::event {

// clang-format off

DECLARE_EVENT(AddEntityEvent, EditorCategory) {
    AddEntityEvent(std::string name)
        : name(std::move(name)) { }

    std::string asString() const override {
        return "[AddEntityEvent] - enttiyName=" + name;
    }

    std::string name;
};

DECLARE_EVENT(SetSkyboxEvent, EditorCategory) {
    SetSkyboxEvent(std::shared_ptr<sl::gfx::Cubemap> cubemap)
        : cubemap(cubemap) {
    }

    std::string asString() const override {
        return "[SetSkyboxEvent]";
    }

    std::shared_ptr<sl::gfx::Cubemap> cubemap;
};

DECLARE_EVENT(SerializeSceneEvent, EditorCategory) {
    SerializeSceneEvent(const std::string& path) : path(path) {
    }
    
    std::string asString() const override {
        return "[SerializeSceneEvent]";
    }

    std::string path;
};

DECLARE_EVENT(DeserializeSceneEvent, EditorCategory) {
    DeserializeSceneEvent(const std::string& path) : path(path) {
    }
    
    std::string asString() const override {
        return "[DeserializeSceneEvent]";
    }

    std::string path;
};

DECLARE_EVENT(ChangeSceneCenterEvent, EditorCategory) {
    ChangeSceneCenterEvent(math::Vec3 center)
        : center(center) { }

    std::string asString() const override {
        return "[ChangeSceneCenterEvent]";
    }

    math::Vec3 center;
};

DECLARE_EVENT(WindowResizedEvent, CoreCategory) {
    WindowResizedEvent(int width, int height)
        : width(width)
        , height(height) {
    }

    std::pair<int, int> getSize() const {
        return { width, height };
    }

    std::string asString() const override {
        return "[WindowResizedEvent]";
    }

    int width;
    int height;
};

DECLARE_EVENT(QuitEvent, CoreCategory) {
    std::string asString() const override {
        return "[QuitEvent]";
    }
};

// clang-format on
}
