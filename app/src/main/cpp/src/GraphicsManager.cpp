#include <cstring>
#include "GraphicsManager.h"
#include "Log.h"

using namespace DroidBlaster::Graphics;

Manager::Manager(android_app *pApplication) : m_application(pApplication),
                                              m_renderWidth(0), m_renderHeight(0),
                                              m_elements(), m_elementCount(0) {
    Log::info("Creating GraphicsManager");
}

Manager::~Manager() {
    Log::info("Destroying GraphicsManager");

    for (auto& r : m_elements) {
        delete r;
    }
}

DroidBlaster::status Manager::start() {
    Log::info("Starting GraphicsManager");

    //Установить 32-битный формат
    ANativeWindow_Buffer windowBuffer;
    if (ANativeWindow_setBuffersGeometry(m_application->window, 0, 0, WINDOW_FORMAT_RGBA_8888) < 0) {
        Log::error("Error while setting buffer geometry");
        return STATUS_KO;
    }
    // Заблокировать буфер окна, чтобы получить его свойства
    if (ANativeWindow_lock(m_application->window, &windowBuffer, nullptr) >= 0) {
        m_renderWidth = windowBuffer.width;
        m_renderHeight = windowBuffer.height;
        ANativeWindow_unlockAndPost(m_application->window);
    } else {
        Log::error("Error while locking window");
        return STATUS_KO;
    }

    return STATUS_OK;
}

DroidBlaster::status Manager::update() {
    // Заблокировать буфер экрана перед рисованием
    ANativeWindow_Buffer windowBuffer;
    if (ANativeWindow_lock(m_application->window, &windowBuffer, nullptr) < 0) {
        Log::error("Error while starting GraphicsManager");
        return STATUS_KO;
    }
    // Очистить окно
    //std::memset(windowBuffer.bits, 0, windowBuffer.stride * windowBuffer.height * sizeof(uint32_t*));
    // Вывести графические элементы
    int32_t maxX = windowBuffer.width - 1;
    int32_t maxY = windowBuffer.height - 1;
    for (int32_t i = 0; i != m_elementCount; ++i) {
        Element* pElement = m_elements[i];

        // Вычислить координаты
        auto leftX = static_cast<int32_t>(pElement->location.x - pElement->width / 2);
        auto rightX = static_cast<int32_t>(pElement->location.x + pElement->width / 2);
        auto leftY = static_cast<int32_t>(windowBuffer.height - pElement->location.y - pElement->height / 2);
        auto rightY = static_cast<int32_t>(windowBuffer.height - pElement->location.y + pElement->height / 2);

        // Обрезать координаты
        if (rightX < 0 || leftX > maxX || rightY < 0 || leftY > maxY) continue;
        if (leftX < 0) {
            leftX = 0;
        } else if (rightX > maxX) {
            rightX = maxX;
        }
        if (leftY < 0) {
            leftY = 0;
        } else if (rightY > maxY) {
            rightY = maxY;
        }
        // Нарисовать прямоугольник
        uint32_t* line = (uint32_t*) (windowBuffer.bits) + (windowBuffer.stride * leftY);
        for (int iY = leftY; iY != rightY; iY++) {
            for (int iX = leftX; iX != rightX; iX++) {
                line[iX] = 0X000000FF;
            }
            line = line + windowBuffer.stride;
        }
    }
    ANativeWindow_unlockAndPost(m_application->window);
    return STATUS_OK;
}

Element *Manager::registerElement(int32_t pHeight, int32_t pWidth) {
    m_elements[m_elementCount] = new Element(pWidth, pHeight);
    return m_elements[m_elementCount++];
}