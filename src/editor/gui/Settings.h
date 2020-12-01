#pragma once

namespace editor::gui {

struct Settings {
    explicit Settings(int viewportWidth, int viewportHeight, float leftPanelWidthRatio,
        float leftSideTopBottomRatio)
        : viewportWidth(viewportWidth)
        , viewportHeight(viewportHeight)
        , leftPanelWidthRatio(leftPanelWidthRatio)
        , leftSideTopBottomRatio(leftSideTopBottomRatio)

    {
    }

    int viewportWidth;
    int viewportHeight;

    float leftPanelWidthRatio;
    float leftSideTopBottomRatio;
};
}
