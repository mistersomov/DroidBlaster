#ifndef DROIDBLASTER_MOVEABLEBODY_HPP
#define DROIDBLASTER_MOVEABLEBODY_HPP

#include "manager/InputManager.hpp"
#include "manager/PhysicsManager.h"
#include "Types.h"

namespace DroidBlaster {
    class MoveableBody {
    public:
        MoveableBody(android_app *pApplication, InputManager &pInputManager,
                     PhysicsManager &pPhysicsManager);

        PhysicsBody *registerMoveableBody(Location &pLocation, int32_t pSizeX, int32_t pSizeY);
        void initialize();
        void update();

    private:
        PhysicsManager& m_physicsManager;
        InputManager& m_inputManager;
        PhysicsBody* m_body;
    };
}

#endif //DROIDBLASTER_MOVEABLEBODY_HPP