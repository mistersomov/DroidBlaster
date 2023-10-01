#pragma once

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <android_native_app_glue.h>

#include "Types.h"
#include "manager/Resource.h"
#include "Sound.hpp"
#include "SoundQueue.hpp"

namespace DroidBlaster {
    class SoundManager {
    public:
        SoundManager(android_app* pApplication);
        ~SoundManager();

        status start();
        void stop();
        status playBGM(Resource& pResource);
        void stopBGM();
        Sound* registerSound(Resource& pResource);
        void playSound(Sound* pSound);

    private:
        android_app* m_application;
        SLObjectItf m_engineObj, m_outputMixObj;
        SLEngineItf m_engine;
        SLObjectItf m_BGMPlayerObj;
        SLPlayItf m_BGMPlayer;
        SLSeekItf m_BGMPlayerSeek;

        static const int32_t QUEUE_COUNT = 4;
        SoundQueue m_soundQueues[QUEUE_COUNT];
        int32_t m_currentQueue;
        Sound* m_sounds[32];
        int32_t m_soundCount;
    };
}