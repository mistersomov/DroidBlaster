#include "manager/SoundManager.h"
#include "manager/Resource.h"
#include "Log.h"

namespace DroidBlaster {
    SoundManager::SoundManager(android_app *pApplication) : m_application(pApplication),
                                                            m_engine(nullptr), m_engineObj(nullptr),
                                                            m_outputMixObj(nullptr) {
        Log::info("Creating SoundManager");
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

        return STATUS_OK;

        ERROR:
        Log::error("Error while starting SoundManager");
        stop();
        return STATUS_KO;
    }

    void SoundManager::stop() {
        Log::info("Stopping SoundManager");

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
}