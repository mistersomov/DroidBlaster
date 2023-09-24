#include "MoveableBody.hpp"

namespace DroidBlaster {
    MoveableBody::MoveableBody(android_app *pApplication,
                               DroidBlaster::InputManager &pInputManager,
                               DroidBlaster::PhysicsManager &pPhysicsManager) : m_inputManager(
            pInputManager), m_physicsManager(pPhysicsManager), m_body(
            nullptr) {}

    PhysicsBody *
    MoveableBody::registerMoveableBody(Location &pLocation, int32_t pSizeX, int32_t pSizeY) {
        m_body = m_physicsManager.loadBody(pLocation, pSizeX, pSizeY);
        m_inputManager.setRefPoint(&pLocation);
        return m_body;
    }

    void MoveableBody::initialize() {
        m_body->velocityX = 0.0f;
        m_body->velocityY = 0.0f;
    }

    void MoveableBody::update() {
        static const float MOVE_SPEED = 320.0f;

        m_body->velocityX = m_inputManager.getDirectionX() * MOVE_SPEED;
        m_body->velocityY = m_inputManager.getDirectionY() * MOVE_SPEED;
    }
}