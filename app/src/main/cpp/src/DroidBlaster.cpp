#include "DroidBlaster.h"
#include "Log.h"
#include <unistd.h>

static const int32_t SHIP_SIZE = 64;

namespace DroidBlaster {
    DroidBlaster::DroidBlaster(android_app *pApplication) : m_eventLoop(pApplication, *this),
                                                            m_graphicsManager(pApplication),
                                                            m_ship(pApplication, m_graphicsManager) {
        Log::info("Creating DroidBlaster");

        Graphics::Element* shipGraphics = m_graphicsManager.registerElement(SHIP_SIZE, SHIP_SIZE);
        m_ship.registerShip(shipGraphics);
    }

    void DroidBlaster::run() {
        m_eventLoop.run();
    }

    status DroidBlaster::onActivate() {
        Log::info("Activating DroidBlaster");

        if (m_graphicsManager.start() != STATUS_OK) {
            return STATUS_KO;
        }
        m_ship.initialize();
        return STATUS_OK;
    }

    void DroidBlaster::onDeactivate() {
        Log::info("Deactivating DroidBlaster");
    }

    status DroidBlaster::onStep() {
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