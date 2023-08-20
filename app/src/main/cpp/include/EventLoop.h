#pragma once

#include <android_native_app_glue.h>

class EventLoop {
public:
    EventLoop(android_app* pApplication);
    ~EventLoop() = default;

    void run();

private:
    android_app* _application;
};