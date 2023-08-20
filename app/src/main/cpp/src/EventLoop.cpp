#include "EventLoop.h"
#include "Log.h"

EventLoop::EventLoop(android_app *pApplication) : _application(pApplication) {}

void EventLoop::run() {
    int32_t result, events;
    android_poll_source *source;

    Log::info("Starting event loop");
    while (true) {
        result = ALooper_pollAll(-1, nullptr, &events, (void **) &source) >= 0;
        // Цикл обработки событий.
        while (result) {
            // Событие для обработки.
            if (source != nullptr) {
                source->process(_application, source);
            }
            // Завершение приложения.
            if (_application->destroyRequested) {
                Log::info("Exiting event loop");
                return;
            }
        }
    }
}