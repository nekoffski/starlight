#pragma once

#include <algorithm>

#include "WidgetProperties.h"

namespace editor::gui {

struct GuiProperties {
    explicit GuiProperties(int windowWidth, int windowHeight) {
        constexpr float f = 1600.0f;
        constexpr float u = 900.0f;

        constexpr float maxFactor = 1.25f;
        constexpr float minFactor = 0.75f;

        const float factorX = std::clamp((float)f / windowWidth, minFactor, maxFactor);
        const float factorY = std::clamp((float)u / windowHeight, minFactor, maxFactor);

        const float leftPanelWidthRatio     = 0.2f * factorX;
        const float leftPanelTopBottomRatio = 0.5f;

        const float scenePanelHeight      = leftPanelTopBottomRatio * windowHeight;
        const float propertiesPanelHeight = windowHeight - scenePanelHeight;
        const float leftPanelWidth        = windowWidth * leftPanelWidthRatio;

        const float bottomPanelHeightRatio = 0.2f * factorY;

        scenePanelProperties = {
            {0,              35                   },
            {leftPanelWidth, scenePanelHeight - 35}
        };

        propertiesPanelProperties = {
            {0,              scenePanelHeight     },
            {leftPanelWidth, propertiesPanelHeight}
        };

        auto bottomPanelHeight = 0.2f * windowHeight;

        bottomPanelProperties = {
            {leftPanelWidth,               windowHeight - bottomPanelHeight},
            {windowWidth - leftPanelWidth, bottomPanelHeight               }
        };

        const float rightPanelWidthRatio = 0.22f * factorX;
        const float rightPanelWidth      = windowWidth * rightPanelWidthRatio;

        rightPanelProperties = {
            {windowWidth - rightPanelWidth, 35                                   },
            {rightPanelWidth,               windowHeight - 35 - bottomPanelHeight}
        };

        const float toolBarHeightRatio = 0.045f;
        const float toolBarHeight      = toolBarHeightRatio * windowHeight;

        toolBarProperties = {
            {leftPanelWidth, windowHeight - bottomPanelHeight - toolBarHeight},
            {0,              toolBarHeight                                   }
        };
    }

    WidgetProperties scenePanelProperties;
    WidgetProperties propertiesPanelProperties;
    WidgetProperties bottomPanelProperties;
    WidgetProperties rightPanelProperties;
    WidgetProperties toolBarProperties;
};

}  // namespace editor::gui