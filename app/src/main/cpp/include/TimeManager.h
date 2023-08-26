#pragma once

#include "Types.h"
#include <ctime>

namespace DroidBlaster {
    class TimeManager {
    public:
        TimeManager();

        void reset();
        void update();
        static double now();
        float elapsed() { return };
        float elapsedTotal() { return };
    private:
        double m_firstTime, m_lastTime;
        float m_elapsed, m_elapsedTotal;
    };
}