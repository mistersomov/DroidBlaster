#pragma once

#include <android_native_app_glue.h>
#include "ActivityHandler.h"
#include "InputHandler.hpp"

namespace DroidBlaster {
    class EventLoop {
    public:
        EventLoop(android_app *pApplication, ActivityHandler &pActivityHandler,
                  InputHandler &pInputHandler);

        ~EventLoop() = default;

        void run();

    private:
        void activate();

        void deactivate();

        void processAppEvent(int32_t pCommand);
        int32_t processInputEvent(AInputEvent* pEvent);

        static void callback_appEvent(android_app *pApplication, int32_t pCommand);
        static int32_t callback_input(android_app* pApplication, AInputEvent* pEvent);

    private:
        android_app *m_application;
        ActivityHandler &m_activityHandler;
        InputHandler &m_inputHandler;
        bool m_enabled;
        bool m_quit;
    };
}