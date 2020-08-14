#pragma once

#include "Component.h"

#include <memory>

#include "starlight/gui/Utils.hpp"
#include "starlight/gui/Window.h"
#include "starlight/rendering/data/ModelRenderData.h"

namespace sl::ecs::component {

struct GridParams {
    int chunksX;
    int chunksZ;
    float chunkWidth;
    float chunkLength;
};

class MeshGridComponent : public Component {
public:
    explicit MeshGridComponent(std::shared_ptr<rendering::data::ModelRenderData> gridRenderData)
        : m_gridRenderData(gridRenderData)
        , m_gridParams{ 3, 3, 1.0f, 1.0f } {
    }

    ComponentType getType() const {
        return ComponentType::MESH_GRID;
    }

    void onGUI(gui::Window& window) override {
        if (window.beginTreeNode("Mesh grid")) {
            window.displayText("Chunk width");
            window.dragFloat(gui::createHiddenLabel("chunk_width"), m_gridParams.chunkWidth, 0.1f, 0.0f, 25.0f);
            window.displayText("Chunk width");
            window.dragFloat(gui::createHiddenLabel("chunk_length"), m_gridParams.chunkLength, 0.1f, 0.0f, 25.0f);
            window.displayText("Chunks x");
            window.dragInt(gui::createHiddenLabel("chunks_x"), m_gridParams.chunksX, 1.0f, 0, 25);
            window.displayText("Chunks z");
            window.dragInt(gui::createHiddenLabel("chunks_z"), m_gridParams.chunksZ, 1.0f, 0, 25);
            window.popTreeNode();
        }
    }

    GridParams getGridParams() const {
        return m_gridParams;
    }

    std::shared_ptr<rendering::data::ModelRenderData> getGridRenderData() {
        return m_gridRenderData;
    }

private:
    std::shared_ptr<rendering::data::ModelRenderData> m_gridRenderData;

    GridParams m_gridParams;
};
}