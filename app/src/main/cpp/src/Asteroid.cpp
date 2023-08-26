#include "Asteroid.h"
#include "Log.h"

static const float BOUNDS_MARGIN = 128;
static const float MIN_VELOCITY = 150.0f, VELOCITY_RANGE = 600.0f;

DroidBlaster::Asteroid::Asteroid(android_app *pApplication, DroidBlaster::TimeManager &pTimeManager,
                                 DroidBlaster::Graphics::Manager &pGraphicsManager,
                                 DroidBlaster::PhysicsManager &pPhysicsManager)
        : m_timeManager(pTimeManager), m_graphicsManager(pGraphicsManager),
          m_physicsManager(pPhysicsManager), m_bodies(), m_bodyCount(0), m_minBound(0.0f),
          m_upperBound(0.0f), m_lowerBound(0.0f), m_leftBound(0.0f), m_rightBound(0.0f) {}

void DroidBlaster::Asteroid::registerAsteroid(DroidBlaster::Location &pLocation, int32_t pSizeX,
                                              int32_t pSizeY) {
    m_bodies[m_bodyCount++] = m_physicsManager.loadBody(pLocation, pSizeX, pSizeY);
}

void DroidBlaster::Asteroid::initialize() {
    m_minBound = static_cast<float>(m_graphicsManager.getRenderHeight());
    m_upperBound = m_minBound * 2;
    m_lowerBound = -BOUNDS_MARGIN;
    m_leftBound = -BOUNDS_MARGIN;
    m_rightBound = static_cast<float>(m_graphicsManager.getRenderWidth()) + BOUNDS_MARGIN;
    for (int32_t i = 0; i != m_bodyCount; ++i) {
        spawn(m_bodies[i]);
    }
}

void DroidBlaster::Asteroid::update() {
    for (int32_t i = 0; i != m_bodyCount; ++i) {
        PhysicsBody *body = m_bodies[i];

        if (body->location->x < m_leftBound
            || body->location->x > m_rightBound
            || body->location->y < m_lowerBound
            || body->location->y > m_upperBound) { spawn(body); }
    }
}

void DroidBlaster::Asteroid::spawn(DroidBlaster::PhysicsBody *pBody) {
    float velocity = -(RAND(VELOCITY_RANGE) + MIN_VELOCITY);
    float posX = RAND(m_graphicsManager.getRenderWidth());
    float posY = RAND(m_graphicsManager.getRenderHeight()) + m_graphicsManager.getRenderHeight();

    pBody->velocityX = 0.0f;
    pBody->velocityY = velocity;
    pBody->location->x = posX;
    pBody->location->y = posY;
}