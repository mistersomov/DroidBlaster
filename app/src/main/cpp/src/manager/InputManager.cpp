#include "manager/InputManager.hpp"
#include "Log.h"
#include <cmath>

namespace DroidBlaster {
    InputManager::InputManager(android_app *pApplication,
                               DroidBlaster::Graphics::Manager &pGraphicsManager)
            : m_application(pApplication),
              m_graphicsManager(pGraphicsManager),
              m_directionX(0.0f), m_directionY(0.0f), m_refPoint(nullptr) {}

    void InputManager::start() {
        Log::info("Starting InputManager");

        m_directionX = 0.0f;
        m_directionY = 0.0f;
        m_scaleFactor = float(m_graphicsManager.getRenderWidth()) /
                        float(m_graphicsManager.getRenderHeight());
    }

    bool InputManager::onTouchEvent(AInputEvent *pEvent) {
        static const float MAX_TOUCH_RANGE = 65.0f; // в игровых единицах

        if (m_refPoint != nullptr) {
            if (AMotionEvent_getAction(pEvent) == AMOTION_EVENT_ACTION_MOVE) {
                auto x = AMotionEvent_getX(pEvent, 0) * m_scaleFactor;
                auto y = float(m_graphicsManager.getRenderHeight() -
                        AMotionEvent_getY(pEvent, 0) * m_scaleFactor);

                // преобразовать в координаты с точкой в отсчета в левом нижнем углу
                // (это необходимо только для lMoveY)
                float moveX = x - m_refPoint->x;
                float moveY = y - m_refPoint->y;
                float moveRange = sqrt((moveX * moveX) + (moveY * moveY));

                if (moveRange > MAX_TOUCH_RANGE) {
                    float cropFactor = MAX_TOUCH_RANGE / moveRange;
                    moveX *= cropFactor;
                    moveY *= cropFactor;
                }

                m_directionX = moveX / MAX_TOUCH_RANGE;
                m_directionY = moveY / MAX_TOUCH_RANGE;
            } else {
                m_directionX = 0.0f;
                m_directionY = 0.0f;
            }
        }
        return true;
    }
}