#ifndef DROIDBLASTER_SOUNDQUEUE_HPP
#define DROIDBLASTER_SOUNDQUEUE_HPP

#include "Sound.hpp"

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

namespace DroidBlaster {
    class SoundQueue {
        SLObjectItf m_playerObj;
        SLPlayItf m_player;
        SLBufferQueueItf m_playerQueue;

    public:
        SoundQueue();

        status initialize(SLEngineItf pEngine, SLObjectItf pOutputMixObj);
        void finalize();
        void playSound(Sound* pSound);
    };
}

#endif //DROIDBLASTER_SOUNDQUEUE_HPP