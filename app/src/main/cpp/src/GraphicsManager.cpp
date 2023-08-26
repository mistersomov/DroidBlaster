#include <cstring>
#include "GraphicsManager.h"
#include "Log.h"


namespace DroidBlaster {
    using namespace Graphics;

    Manager::Manager(android_app *pApplication) : m_application(pApplication),
                                                  m_renderWidth(0), m_renderHeight(0),
                                                  m_elements(), m_elementCount(0),
                                                  m_display(EGL_NO_DISPLAY),
                                                  m_surface(EGL_NO_SURFACE),
                                                  m_context(EGL_NO_CONTEXT) {
        Log::info("Creating GraphicsManager");
    }

    Manager::~Manager() {
        Log::info("Destroying GraphicsManager");

        for (int32_t i = 0; i != m_elementCount; ++i) {
            delete m_elements[i];
        }
    }

    DroidBlaster::status Manager::start() {
        Log::info("Starting GraphicsManager");

        EGLint format, numConfigs, errorResult;
        GLenum status;
        EGLConfig config;

        // Определить требования к экрану. Здесь используется 16-битный режим
        const EGLint DISPLAY_ATTRIBS[] = {
                EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
                EGL_BLUE_SIZE, 5, EGL_GREEN_SIZE, 6, EGL_RED_SIZE, 5,
                EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                EGL_NONE
        };
        // Запросить контекст OpenGL ES2
        const EGLint CONTEXT_ATTRIBS[] = {
                EGL_CONTEXT_CLIENT_VERSION, 2,
                EGL_NONE
        };

        m_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if (m_display == EGL_NO_DISPLAY) goto ERROR;
        if (not eglInitialize(m_display, nullptr, nullptr)) goto ERROR;
        if (not eglChooseConfig(m_display, DISPLAY_ATTRIBS, &config, 1, &numConfigs)
            || numConfigs <= 0) {
            goto ERROR;
        }
        if (not eglGetConfigAttrib(m_display, config, EGL_NATIVE_VISUAL_ID, &format)) goto ERROR;
        ANativeWindow_setBuffersGeometry(m_application->window, 0, 0, format);

        m_surface = eglCreateWindowSurface(m_display, config, m_application->window, nullptr);
        if (m_surface == EGL_NO_SURFACE) goto ERROR;

        m_context = eglCreateContext(m_display, config, nullptr, CONTEXT_ATTRIBS);
        if (m_context == EGL_NO_CONTEXT) goto ERROR;

        if (not eglMakeCurrent(m_display, m_surface, m_surface, m_context)
            || not eglQuerySurface(m_display, m_surface, EGL_WIDTH, &m_renderWidth)
            || not eglQuerySurface(m_display, m_surface, EGL_HEIGHT, &m_renderHeight)
            || m_renderWidth <= 0
            || m_renderHeight <= 0) {
            goto ERROR;
        }
        glViewport(0, 0, m_renderWidth, m_renderHeight);
        glDisable(GL_DEPTH_TEST);

//        //Установить 32-битный формат
//        ANativeWindow_Buffer windowBuffer;
//        if (ANativeWindow_setBuffersGeometry(m_application->window, 0, 0, WINDOW_FORMAT_RGBA_8888) <
//            0) {
//            Log::error("Error while setting buffer geometry");
//            return STATUS_KO;
//        }
//        // Заблокировать буфер окна, чтобы получить его свойства
//        if (ANativeWindow_lock(m_application->window, &windowBuffer, nullptr) >= 0) {
//            m_renderWidth = windowBuffer.width;
//            m_renderHeight = windowBuffer.height;
//            ANativeWindow_unlockAndPost(m_application->window);
//        } else {
//            Log::error("Error while locking window");
//            return STATUS_KO;
//        }
        return STATUS_OK;

        ERROR:
        Log::error("Error while starting GraphicsManager");
        stop();
        return STATUS_KO;
    }

    DroidBlaster::status Manager::update() {
        static float clearColor = 0.0f;
        clearColor += 0.001f;
        glClearColor(clearColor, clearColor, clearColor, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (eglSwapBuffers(m_display, m_surface) != EGL_TRUE) {
            Log::error("Error %d swapping buffers", eglGetError());
            return STATUS_KO;
        } else {
            return STATUS_OK;
        }
//        // Заблокировать буфер экрана перед рисованием
//        ANativeWindow_Buffer windowBuffer;
//        if (ANativeWindow_lock(m_application->window, &windowBuffer, nullptr) < 0) {
//            Log::error("Error while starting GraphicsManager");
//            return STATUS_KO;
//        }
//        // Очистить окно
//        //std::memset(windowBuffer.bits, 0, windowBuffer.stride * windowBuffer.height * sizeof(uint32_t*));
//        // Вывести графические элементы
//        int32_t maxX = windowBuffer.width - 1;
//        int32_t maxY = windowBuffer.height - 1;
//        for (int32_t i = 0; i != m_elementCount; ++i) {
//            Element *pElement = m_elements[i];
//
//            // Вычислить координаты
//            auto leftX = static_cast<int32_t>(pElement->location.x - pElement->width / 2);
//            auto rightX = static_cast<int32_t>(pElement->location.x + pElement->width / 2);
//            auto leftY = static_cast<int32_t>(windowBuffer.height - pElement->location.y -
//                                              pElement->height / 2);
//            auto rightY = static_cast<int32_t>(windowBuffer.height - pElement->location.y +
//                                               pElement->height / 2);
//
//            // Обрезать координаты
//            if (rightX < 0 || leftX > maxX || rightY < 0 || leftY > maxY) continue;
//            if (leftX < 0) {
//                leftX = 0;
//            } else if (rightX > maxX) {
//                rightX = maxX;
//            }
//            if (leftY < 0) {
//                leftY = 0;
//            } else if (rightY > maxY) {
//                rightY = maxY;
//            }
//            // Нарисовать прямоугольник
//            uint32_t *line = (uint32_t *) (windowBuffer.bits) + (windowBuffer.stride * leftY);
//            for (int iY = leftY; iY != rightY; iY++) {
//                for (int iX = leftX; iX != rightX; iX++) {
//                    line[iX] = 0X000000FF;
//                }
//                line = line + windowBuffer.stride;
//            }
//        }
//        ANativeWindow_unlockAndPost(m_application->window);
//        return STATUS_OK;
    }

    Element *Manager::registerElement(int32_t pHeight, int32_t pWidth) {
        m_elements[m_elementCount] = new Element(pWidth, pHeight);
        return m_elements[m_elementCount++];
    }

    void Manager::stop() {
        Log::info("Stopping GraphicsManager");

        // Уничтожить контекст OpenGl
        if (m_display != EGL_NO_DISPLAY) {
            eglMakeCurrent(m_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
            if (m_context != EGL_NO_CONTEXT) {
                eglDestroyContext(m_display, m_context);
                m_context = EGL_NO_CONTEXT;
            }
            if (m_surface != EGL_NO_SURFACE) {
                eglDestroySurface(m_display, m_surface);
                m_surface = EGL_NO_SURFACE;
            }
            eglTerminate(m_display);
            m_display = EGL_NO_DISPLAY;
        }
    }
}