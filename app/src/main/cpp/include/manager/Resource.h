#pragma once

#include "Types.h"
#include <android_native_app_glue.h>

namespace DroidBlaster {
    struct ResourceDescriptor {
        int32_t m_descriptor;
        off_t m_start, m_length;
    };
    class Resource {
    public:
        Resource(android_app* pApplication, const char* pPath);

        const char *getPath() { return m_path; };
        status open();
        void close();
        status read(void* pBuffer, size_t pCount);
        ResourceDescriptor descriptor();

        bool operator==(const Resource& pOther);

    private:
        const char *m_path;
        AAssetManager *m_assetManager;
        AAsset *m_asset;
    };
}