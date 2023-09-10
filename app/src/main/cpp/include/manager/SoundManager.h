#pragma once

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <android_native_app_glue.h>

#include "Types.h"
#include "manager/Resource.h"

namespace DroidBlaster {
    class SoundManager {
    public:
        SoundManager(android_app* pApplication);

        status start();
        void stop();
        status playBGM(Resource& pResource);
        void stopBGM();

    private:
        android_app* m_application;
        SLObjectItf m_engineObj, m_outputMixObj;
        SLEngineItf m_engine;
        SLObjectItf m_BGMPlayerObj;
        SLPlayItf m_BGMPlayer;
        SLSeekItf m_BGMPlayerSeek;
    };
}