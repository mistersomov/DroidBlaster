#include "EventLoop.h"

void android_main(android_app* pApplication) {
    EventLoop(pApplication).run();
}