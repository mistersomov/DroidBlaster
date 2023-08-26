#pragma once

#include "GraphicsManager.h"
#include "TimeManager.h"
#include "Types.h"

namespace DroidBlaster {
    struct PhysicsBody {
        PhysicsBody(Location *pLocation, int32_t pWidth, int32_t pHeight) : location(pLocation),
                                                                            width(pWidth),
                                                                            height(pHeight),
                                                                            velocityX(0.0f),
                                                                            velocityY(0.0f) {};

        Location *location;
        int32_t width, height;
        float velocityX, velocityY;
    };

    class PhysicsManager {
    public:
        PhysicsManager(TimeManager &pTimeManager, Graphics::Manager &pGraphicsManager);

        ~PhysicsManager();

        PhysicsBody *loadBody(Location &pLocation, int32_t pWidth, int32_t pHeight);

        void update();

    private:
        TimeManager &m_timeManager;
        Graphics::Manager &m_graphicsManager;
        PhysicsBody *m_physicsBodies[1024];
        int32_t m_physicsBodyCount;
    };
}