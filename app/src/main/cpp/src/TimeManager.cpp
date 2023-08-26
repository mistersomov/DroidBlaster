#include "TimeManager.h"
#include "Log.h"
#include <cstdlib>
#include <time.h>

namespace DroidBlaster {
    DroidBlaster::TimeManager::TimeManager() : m_firstTime(0.0f), m_lastTime(0.0f), m_elapsed(0.0f),
                                               m_elapsedTotal(0.0f) {
        srand(time(nullptr));
    }

    void DroidBlaster::TimeManager::reset() {
        Log::info("Resetting TimeManager");

        m_elapsed = 0.0f;
        m_firstTime = now();
        m_lastTime = m_firstTime;
    }

    void DroidBlaster::TimeManager::update() {
        double currentTime = now();
        m_elapsed = static_cast<float>(currentTime - m_lastTime);
        m_elapsedTotal = static_cast<float>(currentTime - m_firstTime);
        m_lastTime = currentTime;
    }

    double DroidBlaster::TimeManager::now() {
        timespec timeVal {};
        clock_gettime(CLOCK_MONOTONIC, &timeVal);
        return timeVal.tv_sec + (timeVal.tv_nsec * 1.0e-9);
    }
}