#pragma once

#include "Types.h"
#include <android_native_app_glue.h>
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include "Resource.h"
#include <png.h>

namespace DroidBlaster {
    namespace Graphics {
        struct TextureProperties {
            Resource* textureResource;
            GLint texture;
            int32_t width, height;
        };

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
            void stop();
            status update();

            TextureProperties* loadTexture(Resource& pResource);

        private:
            static void callback_readPng(png_structp pStruct, png_bytep pData, png_size_t pSize);

        private:
            android_app* m_application;
            int32_t m_renderWidth, m_renderHeight;
            EGLDisplay m_display;
            EGLSurface m_surface;
            EGLContext m_context;

            TextureProperties m_textures[32];
            Element* m_elements[1024];
            int32_t m_elementCount, m_textureCount;
        };
    }
}