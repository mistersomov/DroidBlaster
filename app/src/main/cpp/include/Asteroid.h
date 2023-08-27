#pragma once

#include "manager/PhysicsManager.h"
#include "manager/TimeManager.h"
#include "Types.h"

namespace DroidBlaster {
    class Asteroid {
    public:
        Asteroid(android_app *pApplication, TimeManager &pTimeManager,
                 Graphics::Manager &pGraphicsManager, PhysicsManager &pPhysicsManager);

        void registerAsteroid(Location& pLocation, int32_t pSizeX, int32_t pSizeY);
        void initialize();
        void update();

    private:
        void spawn(PhysicsBody* pBody);

    private:
        TimeManager& m_timeManager;
        Graphics::Manager& m_graphicsManager;
        PhysicsManager& m_physicsManager;
        PhysicsBody* m_bodies[1024];
        int32_t m_bodyCount;
        float m_minBound, m_upperBound, m_lowerBound;
        float m_leftBound, m_rightBound;
    };
}