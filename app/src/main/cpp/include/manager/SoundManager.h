#pragma once

#include <SLES/OpenSLES.h>
#include <android_native_app_glue.h>

#include "Types.h"

namespace DroidBlaster {
    class SoundManager {
    public:
        SoundManager(android_app* pApplication);

        status start();
        void stop();

    private:
        android_app* m_application;
        SLObjectItf m_engineObj, m_outputMixObj;
        SLEngineItf m_engine;
    };
}