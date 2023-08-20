#pragma once

#include <android_native_app_glue.h>
#include "ActivityHandler.h"

namespace DroidBlaster {
    class EventLoop {
    public:
        EventLoop(android_app* pApplication, ActivityHandler& pActivityHandler);
        ~EventLoop() = default;

        void run();

    private:
        void activate();
        void deactivate();
        void processAppEvent(int32_t pCommand);
        static void callback_appEvent(android_app* pApplication, int32_t pCommand);

    private:
        android_app* m_application;
        ActivityHandler& m_activityHandler;
        bool m_enabled;
        bool m_quit;
    };
}