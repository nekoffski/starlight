#pragma once

#include <memory>

#include "Scene.h"
#include "sl/gui/Panel.h"
#include "sl/rendering/fwd.h"


namespace sl::scene {

class SceneManager {
public:
    virtual void update(float) = 0;
    virtual void setActiveScene(std::shared_ptr<Scene>) = 0;
    virtual void render() = 0;
    virtual void renderSceneGUI(gui::Panel&) = 0;
    virtual void renderMainGUI(gui::Panel&) = 0;
};
}
