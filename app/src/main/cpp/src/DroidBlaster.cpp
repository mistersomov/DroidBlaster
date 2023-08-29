#include "DroidBlaster.h"
#include "Log.h"
#include <unistd.h>

static const int32_t SHIP_SIZE = 64;
static const int32_t ASTEROID_COUNT = 16;
static const int32_t ASTEROID_SIZE = 64;

namespace DroidBlaster {
    DroidBlaster::DroidBlaster(android_app *pApplication) : m_eventLoop(pApplication, *this),
                                                            m_graphicsManager(pApplication),
                                                            m_ship(pApplication, m_graphicsManager),
                                                            m_timeManager(),
                                                            m_physicsManager(m_timeManager,
                                                                             m_graphicsManager),
                                                            m_asteroids(pApplication, m_timeManager,
                                                                        m_graphicsManager,
                                                                        m_physicsManager),
                                                            m_soundManager(pApplication) {
        Log::info("Creating DroidBlaster");

        Graphics::Element *shipGraphics = m_graphicsManager.registerElement(SHIP_SIZE, SHIP_SIZE);
        m_ship.registerShip(shipGraphics);
        for (int32_t i = 0; i != ASTEROID_COUNT; ++i) {
            Graphics::Element *asteroidGraphics = m_graphicsManager.registerElement(ASTEROID_SIZE,
                                                                                    ASTEROID_SIZE);

            m_asteroids.registerAsteroid(asteroidGraphics->location, ASTEROID_SIZE, ASTEROID_SIZE);
        }
    }

    void DroidBlaster::run() {
        m_eventLoop.run();
    }

    status DroidBlaster::onActivate() {
        Log::info("Activating DroidBlaster");

        if (m_graphicsManager.start() != STATUS_OK) {
            return STATUS_KO;
        }
        if (m_soundManager.start() != STATUS_OK) {
            return STATUS_KO;
        }

        m_asteroids.initialize();
        m_ship.initialize();

        m_timeManager.reset();
        return STATUS_OK;
    }

    void DroidBlaster::onDeactivate() {
        Log::info("Deactivating DroidBlaster");
        m_graphicsManager.stop();
        m_soundManager.stop();
    }

    status DroidBlaster::onStep() {
        m_timeManager.update();
        m_physicsManager.update();

        m_asteroids.update();

        return m_graphicsManager.update();
    }

    void DroidBlaster::onStart() {
        Log::info("onStart");
    }

    void DroidBlaster::onResume() {
        Log::info("onResume");
    }

    void DroidBlaster::onPause() {
        Log::info("onPause");
    }

    void DroidBlaster::onStop() {
        Log::info("onStop");
    }

    void DroidBlaster::onDestroy() {
        Log::info("onDestroy");
    }

    void DroidBlaster::onSaveInstanceState(void **pData, size_t *pSize) {
        Log::info("onSaveInstanceState");
    }

    void DroidBlaster::onConfigurationChanged() {
        Log::info("onConfigurationChanged");
    }

    void DroidBlaster::onLowMemory() {
        Log::info("onLowMemory");
    }

    void DroidBlaster::onCreateWindow() {
        Log::info("onCreateWindow");
    }

    void DroidBlaster::onDestroyWindow() {
        Log::info("onDestroyWindow");
    }

    void DroidBlaster::onGainFocus() {
        Log::info("onGainFocus");
    }

    void DroidBlaster::onLostFocus() {
        Log::info("onLostFocus");
    }
}