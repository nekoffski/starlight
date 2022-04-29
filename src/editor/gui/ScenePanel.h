#pragma once

#include <memory>

#include "Widget.h"

namespace editor::gui {

class ScenePanel : public Widget {
   public:
    explicit ScenePanel(std::shared_ptr<SharedState> sharedState);

    void render() override;

   private:
    int m_entityIndex;
};
}  // namespace editor::gui
