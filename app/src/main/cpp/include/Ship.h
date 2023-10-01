#pragma once

#include "manager/GraphicsManager.h"
#include "manager/SoundManager.h"
#include "Sprite.hpp"
#include "Sound.hpp"

namespace DroidBlaster {
    class Ship {
    public:
        Ship(android_app* pApplication, DroidBlaster::Graphics::Manager& pGraphicsManager, SoundManager& pSoundManager);

        //void registerShip(Graphics::Element* pGraphics);
        void registerShip(Sprite* pGraphics, Sound* pCollisionSound);
        void initialize();
    private:
        Graphics::Manager& m_graphicsManager;
        SoundManager& m_soundManager;
        //Graphics::Element* m_Graphics;
        Sprite* m_Graphics;
        Sound* m_collisionSound;
    };
}