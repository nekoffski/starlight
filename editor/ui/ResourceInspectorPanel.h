#pragma once

#pragma once

#include "starlight/scene/Scene.h"

#include "Core.h"
#include "Console.h"

#include "starlight/renderer/gpu/Texture.h"

class ResourceInspectorPanel {
public:
    explicit ResourceInspectorPanel(
      sl::Scene* scene, UIState* state, Logger* logger
    );

    void render();

private:
    void renderResourceUI(sl::u64 resourceId, ResourceType type);
    void renderTextureUI(sl::Texture* texture);

    sl::Scene* m_scene;
    UIState* m_state;
    Logger* m_logger;
};
