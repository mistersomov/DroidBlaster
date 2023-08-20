#include "EventLoop.h"
#include "Log.h"

EventLoop::EventLoop(android_app *pApplication, ActivityHandler &activityHandler)
        : _application(pApplication), _activityHandler(activityHandler), _enabled(false), _quit(false) {
    _application->userData = this;
    _application->onAppCmd = callback_appEvent;
}

void EventLoop::run() {
    int32_t result, events;
    android_poll_source *source;

    Log::info("Starting event loop");
    while (true) {
        result = ALooper_pollAll(_enabled ? 0 : -1, nullptr, &events, (void **) &source) >= 0;
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
        // Выполнить шаг вычислений в приложении.
        if (_enabled && not _quit) {
            if (_activityHandler.onStep() != STATUS_OK) {
                _quit = true;
                ANativeActivity_finish(_application->activity);
            }
        }
    }
}

void EventLoop::activate() {
    // Активировать визуальный компонент, если окно доступно.
    if (not _enabled && _application->window != nullptr) {
        _enabled = true;
        _quit = false;
        if (_activityHandler.onActivate() != STATUS_OK) {
            goto ERROR;
        }
    }
    return;

    ERROR :
    _quit = true;
    deactivate();
    ANativeActivity_finish(_application->activity);
}

void EventLoop::deactivate() {
    if (_enabled) {
        _activityHandler.onDeactivate();
        _enabled = false;
    }
}

void EventLoop::callback_appEvent(android_app *pApplication, int32_t pCommand) {
    EventLoop& eventLoop = *(EventLoop*) pApplication->userData;
    eventLoop.processAppEvent(pCommand);
}

void EventLoop::processAppEvent(int32_t pCommand) {
    switch (pCommand) {
        case APP_CMD_CONFIG_CHANGED:
            _activityHandler.onConfigurationChanged();
            break;
        case APP_CMD_INIT_WINDOW:
            _activityHandler.onCreateWindow();
            break;
        case APP_CMD_DESTROY:
            _activityHandler.onDestroy();
            break;
        case APP_CMD_GAINED_FOCUS:
            _activityHandler.onGainFocus();
            break;
        case APP_CMD_LOST_FOCUS:
            _activityHandler.onLostFocus();
            break;
        case APP_CMD_LOW_MEMORY:
            _activityHandler.onLowMemory();
            break;
        case APP_CMD_PAUSE:
            _activityHandler.onPause();
            break;
        case APP_CMD_RESUME:
            _activityHandler.onResume();
            break;
        case APP_CMD_SAVE_STATE:
            _activityHandler.onSaveInstanceState(
                    &_application->savedState,
                    &_application->savedStateSize
            );
            break;
        case APP_CMD_START:
            _activityHandler.onStart();
            break;
        case APP_CMD_STOP:
            _activityHandler.onStop();
            break;
        case APP_CMD_TERM_WINDOW:
            _activityHandler.onDestroyWindow();
            deactivate();
            break;
        default:
            break;
    }
}