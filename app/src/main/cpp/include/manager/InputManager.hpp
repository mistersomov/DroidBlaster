#ifndef DROIDBLASTER_INPUTMANAGER_HPP
#define DROIDBLASTER_INPUTMANAGER_HPP

#include "manager/GraphicsManager.h"
#include "InputHandler.hpp"
#include "Types.h"

#include <android_native_app_glue.h>

namespace DroidBlaster {
    class InputManager : public InputHandler {
    public:
        InputManager(android_app *pApplication, Graphics::Manager &pGraphicsManager);

        ~InputManager() {};

        float getDirectionX() { return m_directionX; };
        float getDirectionY() { return m_directionY; };
        void setRefPoint(Location* pRefPoint) { m_refPoint = pRefPoint; };

        void start();

    protected:
        bool onTouchEvent(AInputEvent *pEvent);

    private:
        android_app *m_application;
        Graphics::Manager &m_graphicsManager;

        // значения ввода
        float m_scaleFactor;
        float m_directionX, m_directionY;

        // контрольная точка для вычисления расстояния до точки касания
        Location *m_refPoint;
    };
}

#endif //DROIDBLASTER_INPUTMANAGER_HPP
