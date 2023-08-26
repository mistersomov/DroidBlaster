#include "PhysicsManager.h"
#include "Log.h"

DroidBlaster::PhysicsManager::PhysicsManager(DroidBlaster::TimeManager &pTimeManager,
                                             DroidBlaster::Graphics::Manager &pGraphicsManager)
        : m_timeManager(pTimeManager), m_graphicsManager(pGraphicsManager), m_physicsBodies(),
          m_physicsBodyCount(0) {
    Log::info("Creating PhysicsManager");
}

DroidBlaster::PhysicsManager::~PhysicsManager() {
    Log::info("Destroying PhysicsManager");

    for (auto &p: m_physicsBodies) delete p;
}

DroidBlaster::PhysicsBody *
DroidBlaster::PhysicsManager::loadBody(DroidBlaster::Location &pLocation, int32_t pWidth,
                                       int32_t pHeight) {
    PhysicsBody *body = new PhysicsBody(&pLocation, pWidth, pHeight);
    m_physicsBodies[m_physicsBodyCount++] = body;
    return body;
}

void DroidBlaster::PhysicsManager::update() {
    float timeStep = m_timeManager.elapsed();
    for (auto& pBody : m_physicsBodies) {
        PhysicsBody* body = pBody;

        body->location->x += timeStep * body->velocityX;
        body->location->y += timeStep * body->velocityY;
    }
}