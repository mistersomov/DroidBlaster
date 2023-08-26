#include "Ship.h"
#include "Log.h"
#include "Types.h"

static const float INITAl_X = 0.5f;
static const float INITAl_Y = 0.25f;

namespace DroidBlaster {
    DroidBlaster::Ship::Ship(android_app *pApplication,
                             DroidBlaster::Graphics::Manager &pGraphicsManager) : m_graphicsManager(pGraphicsManager),
                                                                                  m_Graphics(nullptr) {

    }

    void DroidBlaster::Ship::initialize() {
        m_Graphics->location.x = INITAl_X * static_cast<float>(m_graphicsManager.getRenderWidth());
        m_Graphics->location.y = INITAl_Y * static_cast<float>(m_graphicsManager.getRenderHeight());
    }

    void DroidBlaster::Ship::registerShip(DroidBlaster::Graphics::Element *pGraphics) {
        m_Graphics = pGraphics;
    }
}