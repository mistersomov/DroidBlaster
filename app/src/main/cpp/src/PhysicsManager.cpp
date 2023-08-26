#include "PhysicsManager.h"
#include "Log.h"

namespace DroidBlaster {
    DroidBlaster::PhysicsManager::PhysicsManager(DroidBlaster::TimeManager &pTimeManager,
                                                 DroidBlaster::Graphics::Manager &pGraphicsManager)
            : m_timeManager(pTimeManager), m_graphicsManager(pGraphicsManager), m_physicsBodies(),
              m_physicsBodyCount(0) {
        Log::info("Creating PhysicsManager");
    }

    DroidBlaster::PhysicsManager::~PhysicsManager() {
        Log::info("Destroying PhysicsManager");

        for (int32_t i = 0; i != m_physicsBodyCount; ++i) {
            delete m_physicsBodies[i];
        }
    }

    DroidBlaster::PhysicsBody *
    DroidBlaster::PhysicsManager::loadBody(DroidBlaster::Location &pLocation, int32_t pWidth,
                                           int32_t pHeight) {
        auto *body = new PhysicsBody(&pLocation, pWidth, pHeight);
        m_physicsBodies[m_physicsBodyCount++] = body;
        return body;
    }

    void DroidBlaster::PhysicsManager::update() {
        float timeStep = m_timeManager.elapsed();
        for (int32_t i = 0; i != m_physicsBodyCount; ++i) {
            PhysicsBody* body = m_physicsBodies[i];

            body->location->x += timeStep * body->velocityX;
            body->location->y += timeStep * body->velocityY;
        }
    }
}