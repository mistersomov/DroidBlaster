#include "Sound.hpp"
#include "Log.h"

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

namespace DroidBlaster {
    Sound::Sound(android_app *pApplication, Resource *pResource) : m_resource(pResource),
                                                                   m_buffer(nullptr),
                                                                   m_length(0) {}

    const char *Sound::getPath() {
        return m_resource->getPath();
    }

    status Sound::load() {
        Log::info("Loading sound %s", m_resource->getPath());

        status result;
        // Открыть файл
        if (m_resource->open() != STATUS_OK) goto ERROR;

        // Прочитать содержимое файла
        m_length = m_resource->getLength();
        m_buffer = new uint8_t[m_length];
        result = m_resource->read(m_buffer, m_length);
        m_resource->close();

        return STATUS_OK;

        ERROR:
        Log::error("Error while reading PCM sound");
        return STATUS_KO;
    }

    status Sound::unload() {
        delete[] m_buffer;
        m_buffer = nullptr;
        m_length = 0;

        return STATUS_OK;
    }
}