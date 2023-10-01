#include "manager/SoundManager.h"
#include "Log.h"

namespace DroidBlaster {
    SoundManager::SoundManager(android_app *pApplication) : m_application(pApplication),
                                                            m_engine(nullptr), m_engineObj(nullptr),
                                                            m_outputMixObj(nullptr),
                                                            m_BGMPlayerObj(nullptr),
                                                            m_BGMPlayer(nullptr),
                                                            m_BGMPlayerSeek(nullptr),
                                                            m_soundQueues(), m_currentQueue(0),
                                                            m_sounds(), m_soundCount(0) {
        Log::info("Creating SoundManager");
    }

    SoundManager::~SoundManager() {
        Log::info("Destroying SoundManager");

        for (int32_t i = 0; i != m_soundCount; ++i) {
            delete m_sounds[i];
        }
        m_soundCount = 0;
    }

    status SoundManager::start() {
        Log::info("Starting SoundManager");

        SLresult result;
        const SLuint32 engineMixIIDCount = 1;
        const SLInterfaceID engineMixIIDs[] = {SL_IID_ENGINE};
        const SLboolean engineMixReqs[] = {SL_BOOLEAN_TRUE};
        const SLuint32 outputMixIIDCount = 0;
        const SLInterfaceID outputMixIIDs[] = {};
        const SLboolean outputMixReqs[] = {};

        // Создать объект механизма OpenSL ES и настроить его
        result = slCreateEngine(&m_engineObj, 0, nullptr, engineMixIIDCount, engineMixIIDs,
                                engineMixReqs);
        if (result != SL_RESULT_SUCCESS) goto ERROR;

        result = (*m_engineObj)->Realize(m_engineObj, SL_BOOLEAN_FALSE);
        if (result != SL_RESULT_SUCCESS) goto ERROR;

        result = (*m_engineObj)->GetInterface(m_engineObj, SL_IID_ENGINE, &m_engine);
        if (result != SL_RESULT_SUCCESS) goto ERROR;

        // Создать микшер для вывода звука
        result = (*m_engine)->CreateOutputMix(m_engine, &m_outputMixObj, outputMixIIDCount,
                                              outputMixIIDs, outputMixReqs);
        result = (*m_outputMixObj)->Realize(m_outputMixObj, SL_BOOLEAN_FALSE);

        Log::info("Starting sound player");
        for (auto &m_soundQueue: m_soundQueues) {
            if (m_soundQueue.initialize(m_engine, m_outputMixObj) != STATUS_OK) goto ERROR;
        }
        for (int32_t i = 0; i != m_soundCount; ++i) {
            if (m_sounds[i]->load() != STATUS_OK) goto ERROR;
        }

        return STATUS_OK;

        ERROR:
        Log::error("Error while starting SoundManager");
        stop();
        return STATUS_KO;
    }

    void SoundManager::stop() {
        Log::info("Stopping SoundManager");
        stopBGM();

        for (auto & m_soundQueue : m_soundQueues) {
            m_soundQueue.finalize();
        }

        for (int32_t i = 0; i != m_soundCount; ++i) {
            m_sounds[i]->unload();
        }

        if (m_outputMixObj != nullptr) {
            (*m_outputMixObj)->Destroy(m_outputMixObj);
            m_outputMixObj = nullptr;
        }
        if (m_engineObj != nullptr) {
            (*m_engineObj)->Destroy(m_engineObj);
            m_engineObj = nullptr;
            m_engine = nullptr;
        }
    }

    status SoundManager::playBGM(Resource &pResource) {
        SLresult result;
        Log::info("Opening BGM %s", pResource.getPath());

        ResourceDescriptor descriptor = pResource.descriptor();
        if (descriptor.m_descriptor < 0) {
            Log::info("Couldn't open BGM file");
            return STATUS_KO;
        }
        SLDataLocator_AndroidFD dataLocatorIn;
        dataLocatorIn.locatorType = SL_DATALOCATOR_ANDROIDFD;
        dataLocatorIn.fd = descriptor.m_descriptor;
        dataLocatorIn.offset = descriptor.m_start;
        dataLocatorIn.length = descriptor.m_length;

        SLDataFormat_MIME dataFormat;
        dataFormat.formatType = SL_DATAFORMAT_MIME;
        dataFormat.mimeType = nullptr;
        dataFormat.containerType = SL_CONTAINERTYPE_UNSPECIFIED;

        SLDataSource dataSource;
        dataSource.pLocator = &dataLocatorIn;
        dataSource.pFormat = &dataFormat;

        SLDataLocator_OutputMix dataLocatorOut;
        dataLocatorOut.locatorType = SL_DATALOCATOR_OUTPUTMIX;
        dataLocatorOut.outputMix = m_outputMixObj;

        SLDataSink dataSink;
        dataSink.pLocator = &dataLocatorOut;
        dataSink.pFormat = nullptr;

        const SLuint32 bgmPlayerIIDCount = 2;
        const SLInterfaceID bgmPlayerIIDs[] = {SL_IID_PLAY, SL_IID_SEEK};
        const SLboolean bgmPlayerReqs[] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};

        result = (*m_engine)->CreateAudioPlayer(m_engine, &m_BGMPlayerObj, &dataSource, &dataSink,
                                                bgmPlayerIIDCount,
                                                bgmPlayerIIDs, bgmPlayerReqs);
        if (result != SL_RESULT_SUCCESS) goto ERROR;

        result = (*m_BGMPlayerObj)->Realize(m_BGMPlayerObj, SL_BOOLEAN_FALSE);
        if (result != SL_RESULT_SUCCESS) goto ERROR;

        result = (*m_BGMPlayerObj)->GetInterface(m_BGMPlayerObj, SL_IID_PLAY, &m_BGMPlayer);
        if (result != SL_RESULT_SUCCESS) goto ERROR;

        result = (*m_BGMPlayerObj)->GetInterface(m_BGMPlayerObj, SL_IID_SEEK, &m_BGMPlayerSeek);
        if (result != SL_RESULT_SUCCESS) goto ERROR;

        result = (*m_BGMPlayerSeek)->SetLoop(m_BGMPlayerSeek, SL_BOOLEAN_TRUE, 0, SL_TIME_UNKNOWN);
        if (result != SL_RESULT_SUCCESS) goto ERROR;

        result = (*m_BGMPlayer)->SetPlayState(m_BGMPlayer, SL_PLAYSTATE_PLAYING);
        if (result != SL_RESULT_SUCCESS) goto ERROR;

        return STATUS_OK;

        ERROR:
        Log::error("Error playing BGM");
        return STATUS_KO;
    }

    void SoundManager::stopBGM() {
        if (m_BGMPlayer != nullptr) {
            SLuint32 bgmPlayerState;
            (*m_BGMPlayerObj)->GetState(m_BGMPlayerObj, &bgmPlayerState);
            if (bgmPlayerState == SL_OBJECT_STATE_REALIZED) {
                (*m_BGMPlayer)->SetPlayState(m_BGMPlayer, SL_PLAYSTATE_PAUSED);
                (*m_BGMPlayerObj)->Destroy(m_BGMPlayerObj);
                m_BGMPlayerObj = nullptr;
                m_BGMPlayer = nullptr;
                m_BGMPlayerSeek = nullptr;
            }
        }
    }

    Sound *SoundManager::registerSound(Resource &pResource) {
        for (int32_t i = 0; i != m_soundCount; ++i) {
            if (std::strcmp(pResource.getPath(), m_sounds[i]->getPath()) == 0) {
                return m_sounds[i];
            }
        }

        auto* sound = new Sound(m_application, &pResource);
        m_sounds[m_soundCount++] = sound;

        return sound;
    }

    void SoundManager::playSound(Sound *pSound) {
        int32_t currentQueue = ++m_currentQueue;
        SoundQueue& soundQueue = m_soundQueues[currentQueue % QUEUE_COUNT];
        soundQueue.playSound(pSound);
    }
}