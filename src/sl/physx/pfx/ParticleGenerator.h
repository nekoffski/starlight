#pragma once

#include <random>

#include "Particle.h"
#include "ParticleGeneratorSettings.h"
#include "sl/math/Utils.hpp"

namespace sl::physx::pfx {

class ParticleGenerator {
   public:
    std::vector<Particle> generate(const ParticleGeneratorSettings& settings, int n) {
        std::vector<Particle> particles;
        particles.reserve(n);

        std::mt19937 rand(m_rd());
        static std::uniform_real_distribution<> fiDist(0.0f, 2 * std::numbers::pi);
        std::uniform_real_distribution<> radiusDist(0.0f, settings.radius);

        std::uniform_real_distribution<> speedDist(settings.minSpeed, settings.maxSpeed);
        std::uniform_real_distribution<> scaleDist(settings.minScale, settings.maxScale);
        std::uniform_real_distribution<> dir(0.05f, 0.35f);

        std::uniform_real_distribution<> rDist(settings.minColor.r, settings.maxColor.r);
        std::uniform_real_distribution<> gDist(settings.minColor.g, settings.maxColor.g);
        std::uniform_real_distribution<> bDist(settings.minColor.b, settings.maxColor.b);

        while (n-- > 0) {
            auto fi = fiDist(rand);
            auto r  = radiusDist(rand);

            auto pos = math::Vec3{r * cos(fi), 0.0f, r * sin(fi)};

            auto c         = math::Vec3{rDist(rand), gDist(rand), bDist(rand)};
            auto direction = math::Vec3{-pos.x * dir(rand), 1.0f, -pos.z * dir(rand)};

            Particle p;
            p.position        = pos;
            p.speed           = speedDist(rand);
            p.scale           = scaleDist(rand);
            p.deltaScale      = settings.deltaScale;
            p.color           = c;
            p.direction       = direction;
            p.deltaDirection  = math::Vec3{direction.x, 0.0f, direction.z};
            p.directionFactor = settings.directionFactor;

            particles.emplace_back(std::move(p));
        }

        return particles;
    }

   private:
    std::random_device m_rd;
};
}  // namespace sl::physx::pfx
