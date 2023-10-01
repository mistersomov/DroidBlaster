#include "SoundQueue.hpp"
#include "Log.h"

namespace DroidBlaster {

    SoundQueue::SoundQueue() : m_playerObj(nullptr), m_player(nullptr), m_playerQueue(nullptr) {}

    status SoundQueue::initialize(SLEngineItf pEngine, SLObjectItf pOutputMixObj) {
        Log::info("Starting sound player");

        SLresult result;
        // Настроить источник данных
        SLDataLocator_AndroidSimpleBufferQueue dataLocatorIn;
        dataLocatorIn.locatorType = SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE;
        // Не более одного буфера в очереди
        dataLocatorIn.numBuffers = 1;

        SLDataFormat_PCM dataFormat;
        dataFormat.formatType = SL_DATAFORMAT_PCM;
        dataFormat.numChannels = 1; // Моно звук
        dataFormat.samplesPerSec = SL_SAMPLINGRATE_44_1;
        dataFormat.bitsPerSample = SL_PCMSAMPLEFORMAT_FIXED_16;
        dataFormat.containerSize = SL_PCMSAMPLEFORMAT_FIXED_16;
        dataFormat.channelMask = SL_SPEAKER_FRONT_CENTER;
        dataFormat.endianness = SL_BYTEORDER_LITTLEENDIAN;

        SLDataSource dataSource;
        dataSource.pLocator = &dataLocatorIn;
        dataSource.pFormat = &dataFormat;

        SLDataLocator_OutputMix dataLocatorOut;
        dataLocatorOut.locatorType = SL_DATALOCATOR_OUTPUTMIX;
        dataLocatorOut.outputMix = pOutputMixObj;

        SLDataSink dataSink;
        dataSink.pLocator = &dataLocatorOut;
        dataSink.pFormat = nullptr;

        const SLuint32 soundPlayerIIDCount = 2;
        const SLInterfaceID soundPlayerIIDs[] = {SL_IID_PLAY, SL_IID_BUFFERQUEUE};
        const SLboolean soundPlayerReqs[] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};

        result = (*pEngine)->CreateAudioPlayer(pEngine, &m_playerObj, &dataSource, &dataSink,
                                               soundPlayerIIDCount, soundPlayerIIDs,
                                               soundPlayerReqs);
        if (result != SL_RESULT_SUCCESS) goto ERROR;
        result = (*m_playerObj)->Realize(m_playerObj, SL_BOOLEAN_FALSE);
        if (result != SL_RESULT_SUCCESS) goto ERROR;
        result = (*m_playerObj)->GetInterface(m_playerObj, SL_IID_PLAY, &m_player);
        if (result != SL_RESULT_SUCCESS) goto ERROR;
        result = (*m_playerObj)->GetInterface(m_playerObj, SL_IID_BUFFERQUEUE, &m_playerQueue);
        if (result != SL_RESULT_SUCCESS) goto ERROR;
        result = (*m_player)->SetPlayState(m_player, SL_PLAYSTATE_PLAYING);
        if (result != SL_RESULT_SUCCESS) goto ERROR;

        return STATUS_OK;

        ERROR:
        Log::error("Error while starting SoundQueue");
        return STATUS_KO;
    }

    void SoundQueue::finalize() {
        Log::info("Stopping SoundQueue");

        if (m_playerObj != nullptr) {
            (*m_playerObj)->Destroy(m_playerObj);
            m_playerObj = nullptr;
            m_player = nullptr;
            m_playerQueue = nullptr;
        }
    }

    void SoundQueue::playSound(Sound *pSound) {
        SLresult result;
        SLuint32 playerState;
        (*m_playerObj)->GetState(m_playerObj, &playerState);
        if (playerState == SL_OBJECT_STATE_REALIZED) {
            auto *buffer = (int16_t *) pSound->getBuffer();
            off_t length = pSound->getLength();

            // Удалить имеющийся буфер
            result = (*m_playerQueue)->Clear(m_playerQueue);
            if (result != SL_RESULT_SUCCESS) goto ERROR;

            // Воспроизвести новый звук
            result = (*m_playerQueue)->Enqueue(m_playerQueue, buffer, length);
            if (result != SL_RESULT_SUCCESS) goto ERROR;
        }
        return;

        ERROR:
        Log::error("Error trying to play sound");
    }
}