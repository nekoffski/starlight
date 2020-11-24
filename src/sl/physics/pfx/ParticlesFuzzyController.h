#pragma once



#include "sl/ai/FuzzyController.hpp"
#include "sl/math/Utils.hpp"
#include "sl/math/Vector.hpp"
#include "sl/core/log/Logger.h"


namespace sl::physics::pfx {

const std::string ParticleFuzzyControllerSchema = std::string(RES_DIR) + "pfx_fuzzy.fll";

class ParticleFuzzyController : public ai::FuzzyController {
public:
	explicit ParticleFuzzyController() : ai::FuzzyController(ParticleFuzzyControllerSchema) {}

	int calculateParticlesCount(math::Vec3 cameraPositon, math::Vec3 cameraFront, math::Vec3 particleEmitterPosition, unsigned int fps) {
		auto cameraToParticleDirection = cameraPositon - particleEmitterPosition;
		auto cameraToParticleDistance = math::distance(cameraPositon, particleEmitterPosition);
		float viewFactor = -math::dot(cameraFront, math::normalize(cameraToParticleDirection));	

		SL_INFO("Setting fuzzy engine variables");
		setInputVariable(cameraToParticleDistanceFuzzyVariable, cameraToParticleDistance);
		setInputVariable(fpsFuzzyVariable, fps);
		setInputVariable(viewFactorFuzzyVariable, viewFactor);

		SL_INFO("Processing fuzzy engine");
		process();

		SL_INFO("Getting processed value");
		auto particlesCount = getValue<int>(outputFuzzyVariable);
	
		SL_INFO("PFX FUZZY: A={} and FPS={} and D={} --> P={}", viewFactor, fps, cameraToParticleDistance, particlesCount);
		return particlesCount;
	}

private:
	inline static const std::string cameraToParticleDistanceFuzzyVariable = "D";
	inline static const std::string fpsFuzzyVariable = "FPS";
	inline static const std::string viewFactorFuzzyVariable = "A";
	inline static const std::string outputFuzzyVariable = "P";
};
}
