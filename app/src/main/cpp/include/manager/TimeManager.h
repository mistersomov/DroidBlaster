#pragma once

#include "Types.h"
#include <time.h>

namespace DroidBlaster {
    class TimeManager {
    public:
        TimeManager();

        void reset();
        void update();
        static double now();
        float elapsed() { return m_elapsed; }
        float elapsedTotal() { return m_elapsedTotal; }
    private:
        double m_firstTime, m_lastTime;
        float m_elapsed, m_elapsedTotal;
    };
}