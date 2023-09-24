#ifndef DROIDBLASTER_INPUTHANDLER_HPP
#define DROIDBLASTER_INPUTHANDLER_HPP

#include <android/input.h>

namespace DroidBlaster {
    class InputHandler {
    public:
        virtual ~InputHandler() {};

        virtual bool onTouchEvent(AInputEvent* pEvent) = 0;
    };
}

#endif //DROIDBLASTER_INPUTHANDLER_HPP