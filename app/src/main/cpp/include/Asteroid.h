#pragma once

#include "manager/PhysicsManager.h"
#include "manager/TimeManager.h"
#include "Types.h"

#include <vector>

namespace DroidBlaster {
    class Asteroid {
    public:
        Asteroid(Graphics::Manager &pGraphicsManager, PhysicsManager &pPhysicsManager);

        void registerAsteroid(Location& pLocation, int32_t pSizeX, int32_t pSizeY);
        void initialize();
        void update();

    private:
        void spawn(PhysicsBody* pBody);

    private:
        Graphics::Manager& m_graphicsManager;
        PhysicsManager& m_physicsManager;
        std::vector<PhysicsBody*> m_bodies;

        float m_minBound, m_upperBound, m_lowerBound;
        float m_leftBound, m_rightBound;
    };
}