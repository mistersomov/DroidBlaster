#include "EventLoop.h"
#include "Log.h"

namespace DroidBlaster {
    EventLoop::EventLoop(android_app *pApplication, ActivityHandler &pActivityHandler)
            : m_application(pApplication), m_activityHandler(pActivityHandler), m_enabled(false), m_quit(false) {
        m_application->userData = this;
        m_application->onAppCmd = callback_appEvent;
    }

    void EventLoop::run() {
        int32_t result;
        int32_t events;
        android_poll_source *source;

        Log::info("Starting event loop");
        while (true) {
            // Цикл обработки событий.
            while ((result = ALooper_pollAll(m_enabled ? 0 : -1, nullptr, &events, (void **) &source)) >= 0) {
                // Событие для обработки.
                  if (source != nullptr) {
                    Log::info("Processing an event");
                    source->process(m_application, source);
                }
                // Завершение приложения.
                if (m_application->destroyRequested) {
                    Log::info("Exiting event loop");
                    return;
                }
            }
            // Выполнить шаг вычислений в приложении.
            if (m_enabled && not m_quit) {
                if (m_activityHandler.onStep() != STATUS_OK) {
                    m_quit = true;
                    ANativeActivity_finish(m_application->activity);
                }
            }
        }
    }

    void EventLoop::activate() {
        // Активировать визуальный компонент, если окно доступно.
        if (not m_enabled && m_application->window != nullptr) {
            m_enabled = true;
            m_quit = false;
            if (m_activityHandler.onActivate() != STATUS_OK) {
                goto ERROR;
            }
        }
        return;

        ERROR :
        m_quit = true;
        deactivate();
        ANativeActivity_finish(m_application->activity);
    }

    void EventLoop::deactivate() {
        if (m_enabled) {
            m_activityHandler.onDeactivate();
            m_enabled = false;
        }
    }

    void EventLoop::callback_appEvent(android_app *pApplication, int32_t pCommand) {
        EventLoop& eventLoop = *(EventLoop*) pApplication->userData;
        eventLoop.processAppEvent(pCommand);
    }

    void EventLoop::processAppEvent(int32_t pCommand) {
        switch (pCommand) {
            case APP_CMD_CONFIG_CHANGED:
                m_activityHandler.onConfigurationChanged();
                break;
            case APP_CMD_INIT_WINDOW:
                m_activityHandler.onCreateWindow();
                break;
            case APP_CMD_DESTROY:
                m_activityHandler.onDestroy();
                break;
            case APP_CMD_GAINED_FOCUS:
                activate();
                m_activityHandler.onGainFocus();
                break;
            case APP_CMD_LOST_FOCUS:
                m_activityHandler.onLostFocus();
                deactivate();
                break;
            case APP_CMD_LOW_MEMORY:
                m_activityHandler.onLowMemory();
                break;
            case APP_CMD_PAUSE:
                m_activityHandler.onPause();
                deactivate();
                break;
            case APP_CMD_RESUME:
                m_activityHandler.onResume();
                break;
            case APP_CMD_SAVE_STATE:
                m_activityHandler.onSaveInstanceState(
                        &m_application->savedState,
                        &m_application->savedStateSize
                );
                break;
            case APP_CMD_START:
                m_activityHandler.onStart();
                break;
            case APP_CMD_STOP:
                m_activityHandler.onStop();
                break;
            case APP_CMD_TERM_WINDOW:
                m_activityHandler.onDestroyWindow();
                deactivate();
                break;
            default:
                break;
        }
    }
}