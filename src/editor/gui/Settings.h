#pragma once

#include "Widget.h"

namespace editor::gui {

struct Settings {
    explicit Settings(int viewportWidth, int viewportHeight, float leftPanelWidthRatio,
        float leftSideTopBottomRatio) {
        auto leftPanelWidth = viewportWidth * leftPanelWidthRatio;
        auto scenePanelHeight = viewportHeight * leftSideTopBottomRatio;
        auto propertiesPanelHeight = viewportHeight - scenePanelHeight;

        scenePanelPosition = {
            { 0, 35 },
            { leftPanelWidth, scenePanelHeight - 35 }
        };

        propertiesPanelPosition = {
            { 0, scenePanelHeight },
            { leftPanelWidth, propertiesPanelHeight }
        };

        auto bottomPanelHeight = 0.2f * viewportHeight;

        bottomPanelPosition = {
            { leftPanelWidth, viewportHeight - bottomPanelHeight },
            { viewportWidth - leftPanelWidth, bottomPanelHeight }
        };
    }

    WidgetPosition scenePanelPosition;
    WidgetPosition propertiesPanelPosition;
    WidgetPosition bottomPanelPosition;
};
}
