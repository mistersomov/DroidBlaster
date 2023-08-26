#pragma

#include "Types.h"
#include <android_native_app_glue.h>

namespace DroidBlaster {
    namespace Graphics {
        struct Element {
            Element(int32_t pWidth, int32_t pHeight) : location(), width(pWidth), height(pHeight) {};

            Location location;
            int32_t width, height;
        };

        class Manager {
        public:
            Manager(android_app* pApplication);
            ~Manager();

            int32_t getRenderWidth() { return m_renderWidth; }
            int32_t getRenderHeight() { return m_renderHeight; }
            Element* registerElement(int32_t pHeight, int32_t pWidth);
            status start();
            status update();

        private:
            android_app* m_application;
            int32_t m_renderWidth;
            int32_t m_renderHeight;
            int32_t m_elementCount;
            Element* m_elements[1024];
        };
    }
}