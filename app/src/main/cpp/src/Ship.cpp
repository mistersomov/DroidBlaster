#include "Ship.h"
#include "Log.h"
#include "Types.h"

static const float INITAl_X = 0.5f;
static const float INITAl_Y = 0.25f;

DroidBlaster::Ship::Ship(android_app *pApplication,
                         DroidBlaster::Graphics::Manager &pGraphicsManager) : m_graphicsManager(pGraphicsManager),
                         m_Graphics(nullptr) {

}

void DroidBlaster::Ship::initialize() {
    m_Graphics->location.x = INITAl_X * m_graphicsManager.getRenderWidth();
    m_Graphics->location.y = INITAl_Y * m_graphicsManager.getRenderHeight();
}

void DroidBlaster::Ship::registerShip(DroidBlaster::Graphics::Element *pGraphics) {
    m_Graphics = pGraphics;
}