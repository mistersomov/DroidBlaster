#pragma once

#include "manager/GraphicsManager.h"

namespace DroidBlaster {
    class Ship {
    public:
        Ship(android_app* pApplication, DroidBlaster::Graphics::Manager& pGraphicsManager);

        void registerShip(Graphics::Element* pGraphics);
        void initialize();
    private:
        Graphics::Manager& m_graphicsManager;
        Graphics::Element* m_Graphics;
    };
}