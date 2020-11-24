#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "Skybox.h"
#include "sl/ecs/Registry.h"
#include "sl/ecs/fwd.h"
#include "sl/rendering/camera/Camera.h"

namespace sl::scene {

class Scene {
public:
	virtual ~Scene() = default;


    template <typename Component, typename Factory>
    void setComponentWrapperFactory() {
        m_ecsRegistry.setComponentWrapperFactory<Component, Factory>();
    }

    std::shared_ptr<ecs::Entity> addEntity(std::string name) {
        return m_ecsRegistry.createEntity(std::move(name));
    }

	void setCamera(std::shared_ptr<rendering::camera::Camera> camera) {
		m_camera = std::move(camera);
	}
	
protected:
	std::shared_ptr<rendering::camera::Camera> m_camera;
    ecs::Registry m_ecsRegistry;
};
}
