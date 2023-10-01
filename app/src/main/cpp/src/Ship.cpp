#include "Ship.h"
#include "Log.h"
#include "Types.h"

static const float INITAl_X = 0.5f;
static const float INITAl_Y = 0.25f;

namespace DroidBlaster {
    Ship::Ship(android_app *pApplication,
               DroidBlaster::Graphics::Manager &pGraphicsManager,
               SoundManager &pSoundManager) : m_graphicsManager(pGraphicsManager),
                                              m_soundManager(pSoundManager),
                                              m_Graphics(),
                                              m_collisionSound(nullptr) {}

    void Ship::initialize() {
        m_Graphics->location.x = INITAl_X * static_cast<float>(m_graphicsManager.getRenderWidth());
        m_Graphics->location.y = INITAl_Y * static_cast<float>(m_graphicsManager.getRenderHeight());
        m_soundManager.playSound(m_collisionSound);
    }

    void Ship::registerShip(Sprite *pGraphics, Sound* pCollisionSound) {
        m_Graphics = pGraphics;
        m_collisionSound = pCollisionSound;
    }
}