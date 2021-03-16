#pragma once

#include "WidgetProperties.h"

namespace editor::gui {

struct GuiProperties {
    explicit GuiProperties(int windowWidth, int windowHeight) {
        constexpr float leftPanelWidthRatio = 0.2f;
        constexpr float leftPanelTopBottomRatio = 0.5f;

        const float scenePanelHeight = leftPanelTopBottomRatio * windowHeight;
        const float propertiesPanelHeight = windowHeight - scenePanelHeight;
        const float leftPanelWidth = windowWidth * leftPanelWidthRatio;

        constexpr float bottomPanelHeightRatio = 0.2f;

        scenePanelProperties = {
            { 0, 35 },
            { leftPanelWidth, scenePanelHeight - 35 }
        };

        propertiesPanelProperties = {
            { 0, scenePanelHeight },
            { leftPanelWidth, propertiesPanelHeight }
        };

        auto bottomPanelHeight = 0.2f * windowHeight;

        bottomPanelProperties = {
            { leftPanelWidth, windowHeight - bottomPanelHeight },
            { windowWidth - leftPanelWidth, bottomPanelHeight }
        };

        constexpr float rightPanelWidthRatio = 0.2f;
        const float rightPanelWidth = windowWidth * rightPanelWidthRatio;

        rightPanelProperties = {
            { windowWidth - rightPanelWidth, 35 },
            { rightPanelWidth, windowHeight - 35 - bottomPanelHeight }
        };
    }

    WidgetProperties scenePanelProperties;
    WidgetProperties propertiesPanelProperties;
    WidgetProperties bottomPanelProperties;
    WidgetProperties rightPanelProperties;
};

}