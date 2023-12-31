#pragma once

#include "ActivityHandler.h"
#include "EventLoop.h"
#include "Types.h"
#include "Ship.h"
#include "manager/TimeManager.h"
#include "manager/PhysicsManager.h"
#include "manager/GraphicsManager.h"
#include "manager/SoundManager.h"
#include "manager/Resource.h"
#include "manager/InputManager.hpp"
#include "MoveableBody.hpp"
#include "Asteroid.h"
#include "SpriteBatch.hpp"
#include "StarField.hpp"

namespace DroidBlaster {
    class DroidBlaster : public ActivityHandler {
    public:
        DroidBlaster(android_app* pApplication);

        void run();

    protected:
        status onActivate();
        void onDeactivate();
        status onStep();
        void onStart();
        void onResume();
        void onPause();
        void onStop();
        void onDestroy();
        void onSaveInstanceState(void **pData, size_t *pSize);
        void onConfigurationChanged();
        void onLowMemory();
        void onCreateWindow();
        void onDestroyWindow();
        void onGainFocus();
        void onLostFocus();

    private:
        EventLoop m_eventLoop;
        Graphics::Manager m_graphicsManager;
        TimeManager m_timeManager;
        PhysicsManager m_physicsManager;
        SoundManager m_soundManager;
        InputManager m_inputManager;

        Asteroid m_asteroids;
        Ship m_ship;
        SpriteBatch m_spriteBatch;
        StarField m_starField;

        Resource m_asteroidTexture;
        Resource m_shipTexture;
        Resource m_bgm;
        Resource m_collisionSound;
        Resource m_starTexture;

        MoveableBody m_moveableBody;
    };
}