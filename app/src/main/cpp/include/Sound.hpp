#ifndef DROIDBLASTER_SOUND_HPP
#define DROIDBLASTER_SOUND_HPP

#include "manager/Resource.h"
#include "Types.h"

namespace DroidBlaster {
    class SoundManager;

    class Sound {
    public:
        Sound(android_app *pApplication, Resource *pResource);

        const char *getPath();
        uint8_t *getBuffer() { return m_buffer; };
        off_t getLength() { return m_length;};
        status load();
        status unload();

    private:
        friend class SoundManager;

        Resource *m_resource;
        uint8_t *m_buffer;
        off_t m_length;
    };
}

#endif //DROIDBLASTER_SOUND_HPP