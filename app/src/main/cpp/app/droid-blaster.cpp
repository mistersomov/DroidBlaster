#include "DroidBlaster.h"
#include "EventLoop.h"
#include "Log.h"

void android_main(android_app* pApplication) {
    DroidBlaster(pApplication).run();
}