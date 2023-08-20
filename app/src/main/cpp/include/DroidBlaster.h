#pragma once

#include "ActivityHandler.h"
#include "EventLoop.h"
#include "Types.h"

class DroidBlaster : public ActivityHandler {
public:
    DroidBlaster(android_app* pApplication);

    void run();

protected:
    status onActivate();
    void onDeactivate();
    status onStep();
    void onStart();
    void onResume();
    void onPause();
    void onStop();
    void onDestroy();
    void onSaveInstanceState(void **pData, size_t *pSize);
    void onConfigurationChanged();
    void onLowMemory();
    void onCreateWindow();
    void onDestroyWindow();
    void onGainFocus();
    void onLostFocus();

private:
    EventLoop _eventLoop;
};